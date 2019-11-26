//
//  AcademyFlappyBots.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#include "AcademyFlappyBots.hpp"
#include "GameManager.hpp"

AcademyFlappyBots::AcademyFlappyBots(){};
AcademyFlappyBots::~AcademyFlappyBots(){
    this->unschedule();
};
AcademyFlappyBots* AcademyFlappyBots::instance = 0;

AcademyFlappyBots* AcademyFlappyBots::getInstance(){

    if(instance == 0){
        instance = new AcademyFlappyBots();
    } 
    return instance;
}

void AcademyFlappyBots::setScene(TrainingScene* scene){
    this->scene = scene;
    if(this->scene){
        this->initAcademy();
    }
    else{
        log("Academy not initialized. Null Training scene.");
    }
  
}

void AcademyFlappyBots::initAcademy(){
    this->initPool();
    this->schedule();
}


void AcademyFlappyBots::initPool(){
    this->agentsPool->reserve(this->generationSize);
    for(int i = 0; i< this->generationSize; i++){
        auto agent = AgentFlappyBot::create();
        this->agentsPool->pushBack(agent);
        this->scene->addChild(agent);
        agent->stopAnimation();
        agent->getPhysicsBody()->setGravityEnable(true);
        this->initializeWeights(agent);
        this->setMutation(agent);
    }
}

void AcademyFlappyBots::initializeWeights(AgentFlappyBot* agent){
    vector<float> resp;
    float weight_1 = ((float) rand()) / (float) RAND_MAX;
    float weight_2 = ((float) rand()) / (float) RAND_MAX;
    float weight_3 = ((float) rand()) / (float) RAND_MAX;
    resp.reserve(3);
    resp.push_back(weight_1);
    resp.push_back(weight_2);
    resp.push_back(weight_3);
    agent->setWeights(resp);
}

void AcademyFlappyBots::schedule(){
   Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(AcademyFlappyBots::update, this), this, 1.0f / 60, false, "academy");
}

void AcademyFlappyBots::unschedule(){
     Director::getInstance()->getScheduler()->unschedule("academy", this);
}

void AcademyFlappyBots::tempCalculate(){
    if(this->generationSize > this->agentsPool->size()){
        log("Problems with agentsPool. Problably not initialied.");
        return;
    }
    
    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        auto obs = agent->collectObservations();
        auto weights = agent->getWeights();
        float out = (obs.at(0)*weights.at(0) + obs.at(1)*weights.at(1)  + obs.at(2)*weights.at(2)/weights.size());
        agent->action(out);
    }
    
}

bool AcademyFlappyBots::checkGenerationFinished(){
    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        if(!agent->getIsDead()) return false;
    }
    return true;
}

bool AcademyFlappyBots::checkTrainingFinished(){
    this->currentGeneration += 1;
    bool resp = this->currentGeneration > this->numerOfNegerations;
    return resp;
}

AgentFlappyBot* AcademyFlappyBots::getBestAgent(){
    int bestScore = 0;
    AgentFlappyBot* bestAgent;
     for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        auto agentScore = agent->getTotalScore();
        if(bestScore <= agentScore){
            bestScore = agentScore;
            bestAgent = agent;
        }
    }
    return bestAgent;
}

void AcademyFlappyBots::setMutation(AgentFlappyBot* agent){
    auto weights = agent->getWeights();
    int randIdx = rand() % 3; 
    bool isNegative = rand() % 2;
    float newWeight = ((float) rand())/(float)RAND_MAX;
    if(isNegative){
        newWeight *= -1;
    }
    weights.at(randIdx) = newWeight;
    agent->setWeights(weights);
}

void AcademyFlappyBots::nextGeneration(){
    auto bestAgent = this->getBestAgent();

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        agent->reset();
        this->copyBestWeights(bestAgent,agent);
    }

    //to preserve some percent agent with the bests weights from last generation
    int remainAmount = (int)floor(this->generationSize*0.2);
    // log("Remaning: %d",remainAmount);

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        if(i >= remainAmount){
            this->setMutation(agent);
        }
    }
}

void AcademyFlappyBots::copyBestWeights(AgentFlappyBot* bestAgent,AgentFlappyBot* agent){
    agent->setWeights(bestAgent->getWeights());
}

void AcademyFlappyBots::update(float dt){
    if(GameManager::getInstance()->state != GameManager::PLAYING_STATE) return;

    auto generationFinished = this->checkGenerationFinished();
    if(generationFinished){
        if(this->checkTrainingFinished()){
            this->scene->gameOver();
            GameManager::getInstance()->state = GameManager::FINISHED_STATE;
            return;
        }
        else{
            this->scene->gameOver();
            this->nextGeneration();
            this->scene->restartGame();
        }        
    }

    this->tempCalculate();
}

