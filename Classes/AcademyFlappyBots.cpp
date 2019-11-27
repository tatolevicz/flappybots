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
    resp.reserve(agent->numberOfWeights);
    for(int i = 0; i <agent->numberOfWeights; i++){
        float weight = ((float) rand()) / (float) RAND_MAX;
        weight *= this->weightMagnitude;
        bool isNegative = rand() % 2;
        if(isNegative){
            weight *= -1;
        }
        resp.push_back(weight);
    }
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

        if(agent->getIsDead()) continue;

        auto obs = agent->collectObservations();
        auto weights = agent->getWeights();

        // float out1 = round((obs.at(0)*weights.at(0) + obs.at(1)*weights.at(1)  + obs.at(2)*weights.at(2)));
        // float out2 = round((obs.at(0)*weights.at(3) + obs.at(1)*weights.at(4)  + obs.at(2)*weights.at(5)));
        // float out3 = round((obs.at(0)*weights.at(6) + obs.at(1)*weights.at(7)  + obs.at(2)*weights.at(8)));
        // float out = round(out1*weights.at(9) + out2*weights.at(10) + out3*weights.at(11));
        // float out = round(out1*weights.at(9) + out2*weights.at(10) + out3*weights.at(11));
        float out1 = round(obs.at(0)*weights.at(0));
        float out2 = round(obs.at(1)*weights.at(1));  
        float out3 = round(obs.at(2)*weights.at(2));
        float out4 = round((out1 + out2 + out3)*weights.at(3));
        float out5 = round((out1 + out2 + out3)*weights.at(4));  
        float out6 = round((out1 + out2 + out3)*weights.at(5));
        float out = round((out4*weights.at(6) + out5*weights.at(7) + out6*weights.at(8)));
        agent->action(round(out));
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
    auto weights = bestAgent->getWeights();
    log("Best Agent:");
    for(int i = 0; i < bestAgent->numberOfWeights; i++){
        log("%3.2f",weights.at(i));
    }
    log("Score: %d",bestAgent->getTotalScore());
    return bestAgent;
}

void AcademyFlappyBots::setMutation(AgentFlappyBot* agent){
    auto weights = agent->getWeights();
    int randIdx = rand() % 3; 
    bool isNegative = rand() % 2;
    float newWeight = ((float)rand())/(float)RAND_MAX;
    newWeight *= this->weightMagnitude;
    newWeight *= 2;
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
    int remainAmount = (int)floor(this->generationSize*0.05);
    log("Remaning: %d",remainAmount);

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        if(i >= remainAmount){
            this->setMutation(agent);
        }
    }
}

void AcademyFlappyBots::copyBestWeights(AgentFlappyBot* bestAgent,AgentFlappyBot* agent){
    auto weights = bestAgent->getWeights();
    agent->setWeights(weights);
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

