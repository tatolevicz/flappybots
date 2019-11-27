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

    resp.push_back(-0.64);
    resp.push_back(-0.01);
    resp.push_back(-0.90);
    resp.push_back(0.35);
    resp.push_back(-0.04);
    resp.push_back(-0.06);
    resp.push_back(0.62);
    resp.push_back(-0.50);
    resp.push_back(-0.37);
    resp.push_back(0.35);
    resp.push_back(-0.11);
    resp.push_back(-0.19);

    if(!this->lastBestAgent){
        this->lastBestAgent = AgentFlappyBot::create();
        this->lastBestAgent->retain();
        this->lastBestAgent->setWeights(resp);
        this->lastBestAgent->setTotalScore(5); 
    }

    // for(int i = 0; i <agent->numberOfWeights; i++){
    //     float weight = ((float) rand()) / (float) RAND_MAX;
    //     weight *= this->weightMagnitude;
    //     bool isNegative = rand() % 2;
    //     if(isNegative){
    //         weight *= -1;
    //     }
    //     resp.push_back(weight);
    // }
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

        float out1 = obs.at(0)*weights.at(0);
        float out2 = obs.at(1)*weights.at(1);  
        float out3 = obs.at(2)*weights.at(2);

        float out10 = this->activationFunction(out1 + out2 + out3);

        float out4 = obs.at(0)*weights.at(3);
        float out5 = obs.at(1)*weights.at(4);  
        float out6 = obs.at(2)*weights.at(5);

        float out11 = this->activationFunction(out4 + out5 + out6);

        float out7 = obs.at(0)*weights.at(6);
        float out8 = obs.at(1)*weights.at(7);  
        float out9 = obs.at(2)*weights.at(8);

        float out12 = this->activationFunction(out7 + out8 + out9);

        float out = this->activationFunction((out10*weights.at(9) + out11*weights.at(10) + out12*weights.at(11)));
        
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
    auto weights = bestAgent->getWeights();
    return bestAgent;
}

void AcademyFlappyBots::setMutation(AgentFlappyBot* agent){
    auto weights = agent->getWeights();
    int numberOfMutations = agent->numberOfWeights*0.1;
    for(int i = 0; i < numberOfMutations; i++){
        int randIdx = rand() % agent->numberOfWeights; 
        bool isNegative = rand() % 2;
        float newWeight = ((float)rand())/(float)RAND_MAX;
        newWeight *= this->weightMagnitude;
        if(isNegative){
            newWeight *= -1;
        }
        weights.at(randIdx) = newWeight;
    } 
    agent->setWeights(weights);
}

void AcademyFlappyBots::nextGeneration(){
    
    auto bestAgent = this->getBestAgent();
    AgentFlappyBot* sonAgent = this->permuteGenes(bestAgent,this->lastBestAgent);
    //save weights from de best agent of this generation
    if(this->lastBestAgent->getTotalScore() <= bestAgent->getTotalScore()){
        this->lastBestAgent->setWeights(bestAgent->getWeights());
        this->lastBestAgent->setTotalScore(bestAgent->getTotalScore());
        auto weights = bestAgent->getWeights();
        log("Best Agent:");
        for(int i = 0; i < bestAgent->numberOfWeights; i++){
            log("%3.2f",weights.at(i));
        }
        log("Score: %d",bestAgent->getTotalScore());
    }
    
    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        agent->reset();
        this->copyBestWeights(sonAgent,agent);
    }

    //to preserve some percent agent with the bests weights from last generation
    int remainAmount = (int)floor(this->generationSize*0.05);
    // log("Remaning: %d",remainAmount);

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        if(i >= remainAmount){
            this->setMutation(agent);
        }
        else{
            agent->setWeights(bestAgent->getWeights());
        }
    }
}

AgentFlappyBot* AcademyFlappyBots::permuteGenes(AgentFlappyBot* father, AgentFlappyBot* mother){
    auto son = AgentFlappyBot::create();
    auto fatherWeights = father->getWeights();
    auto motherWeights = mother->getWeights();
    vector<float> sonWeights;
    sonWeights.reserve(father->numberOfWeights);
    int parOuImpar = rand() % 2;
    for(int i = 0; i < father->numberOfWeights; i++){
        if(i%2 == parOuImpar){
            sonWeights.push_back(father->getWeights().at(i));
        }
        else{
            sonWeights.push_back(mother->getWeights().at(i));
        }
    }
    son->setWeights(sonWeights);
    return son;
}
    


void AcademyFlappyBots::copyBestWeights(AgentFlappyBot* bestAgent,AgentFlappyBot* agent){
    auto weights = bestAgent->getWeights();
    agent->setWeights(weights);
}

void AcademyFlappyBots::update(float dt){
    if(GameManager::getInstance()->state != GameManager::PLAYING_STATE) return;

    this->currentTime += dt;

    if(this->currentTime < this->timeToCollect)
    return;

    currentTime = 0;

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

float AcademyFlappyBots::activationFunction(float neuron){
    float resp = 1/(1 + expf(-neuron));
    return resp;
}


