//
//  GeneticAlgorithm.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 01/12/19.
//

#include "GeneticAlgorithm.hpp"


GeneticAlgorithm::GeneticAlgorithm(){};
GeneticAlgorithm::~GeneticAlgorithm(){};

bool GeneticAlgorithm::init(){
    this->initPool();
    return true;
}

void GeneticAlgorithm::initPool(){
    this->agentsPool->reserve(this->generationSize);
    for(int i = 0; i< this->generationSize; i++){
        auto agent = AgentFlappyBot::create();
        this->agentsPool->pushBack(agent);
        agent->stopAnimation();
        agent->getPhysicsBody()->setGravityEnable(true);
        agent->fixedX = (((float)rand())/(float)RAND_MAX) * 600 + 100;

    }

    bestAgentOfAllTime = AgentFlappyBot::create();
    this->bestAgentOfAllTime->retain();
    bestAgentOfAllTime->isDead = true;
    bestAgentOfAllTime->stopAnimation();
}

void GeneticAlgorithm::setMutation(AgentFlappyBot* agent){
    auto weights = agent->getWeights();
    int numberOfMutations = agent->getNumberOfWeights()*0.4;
    vector<int> randVec;
    randVec.reserve(numberOfMutations);
    
    int j = 0;
    while(j < numberOfMutations){
        int randIdx = rand() % agent->getNumberOfWeights();
        if(!this->containsIdx(randVec,randIdx)){
            randVec.push_back(randIdx);
            j++;
        }
    }

    for(int i = 0; i < numberOfMutations; i++){
        bool isNegative = rand() % 2;
        float newWeight = ((float)rand())/(float)RAND_MAX;
        if(isNegative){
            newWeight *= -1;
        }
        weights.at(randVec.at(i)) = newWeight;
    }
    agent->setWeights(weights);
}

AgentFlappyBot* GeneticAlgorithm::permuteGenes(AgentFlappyBot* father, AgentFlappyBot* mother){
    auto son = AgentFlappyBot::create();
    auto fatherWeights = father->getWeights();
    auto motherWeights = mother->getWeights();
    vector<float> sonWeights;
    sonWeights.reserve(father->getNumberOfWeights());
    int parOuImpar = rand() % 2;
    for(int i = 0; i < father->getNumberOfWeights(); i++){
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

void GeneticAlgorithm::copyBestWeights(AgentFlappyBot* fromAgent,AgentFlappyBot* toAgent){
    auto weights = fromAgent->getWeights();
    toAgent->setWeights(weights);
}
    
void GeneticAlgorithm::nextGeneration(){
    auto bestAgents = this->getBestTwoAgents();
    AgentFlappyBot* sonAgent = this->permuteGenes(bestAgents.at(0),bestAgents.at(1));
    
    //################
    // to preserve some percent agent with the bests weights from last generation
    int remainAmount = 2;//(int)floor(this->generationSize*0.05);
    
    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        agent->reset();
        if(i >= remainAmount){
            this->copyBestWeights(sonAgent,agent);
            this->setMutation(agent);
        }
        else{
            this->copyBestWeights(bestAgents.at(i%2==0?0:1),agent);
        }
    }

    this->currentGeneration += 1;
}

Vector<AgentFlappyBot*> GeneticAlgorithm::getBestTwoAgents(){
    float firstBestScore = 0;
    float secondBestScore = 0;

    Vector<AgentFlappyBot*> agents;
    agents.reserve(2);

    AgentFlappyBot* firstAgent = AgentFlappyBot::create();
    AgentFlappyBot* secondAgent = AgentFlappyBot::create();
    
    AgentFlappyBot* firstBestAgent;
    AgentFlappyBot* secondBestAgent;

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        auto agentScore = agent->getLifeTime();
        if(agentScore >= firstBestScore){
            firstBestScore = agentScore;
            firstBestAgent = agent;
        }
    }

    auto firstWeights = firstBestAgent->getWeights();
    firstAgent->setWeights(firstWeights);
    

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        auto agentScore = agent->getLifeTime();
        if(agentScore >= secondBestScore && agent != firstAgent){
            secondBestScore = agentScore;
            secondBestAgent = agent;
        }
    }

    auto secondWeights = secondBestAgent->getWeights();
    secondAgent->setWeights(secondWeights);

    //save score from de best agent of this generation
    if(this->bestAgentOfAllTime->getLifeTime() <= firstAgent->getLifeTime()){
        this->bestAgentOfAllTime->setWeights(firstAgent->getWeights());
        this->bestAgentOfAllTime->setLifeTime(firstAgent->getLifeTime());
    }

    agents.pushBack(firstAgent);
    agents.pushBack(secondAgent);
    
    return agents;
}
bool GeneticAlgorithm::checkGenerationFinished(){
    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        if(!agent->getIsDead()) return false;
    }
    return true;
}
bool GeneticAlgorithm::checkTrainingFinished(){
    bool resp = this->currentGeneration > this->numerOfNegerations;
    return resp;
}

//helper functions
bool GeneticAlgorithm::containsIdx(vector<int> vec,int idx){
    for(int i = 0; i < vec.size(); i++){
        if(vec.at(i) == idx){
            return true;
        }
    }
    return false;
}

bool GeneticAlgorithm::getIsInitialized(){
    return this->isInitialized;
}

void GeneticAlgorithm::setIsInitialized(bool val){
    this->isInitialized = val;
}

AgentFlappyBot* GeneticAlgorithm::getBestAgent(){
    return this->getBestTwoAgents().at(1);
}

float GeneticAlgorithm::getTimeCurrentGeneration(){
    float resp = 0;
//    float time = 0;
    for(int i = 0; i < agentsPool->size();i++){
        float agentTime = agentsPool->at(i)->getLifeTime();
//        if(agentTime >= time){
        resp += agentTime;
//        time = agentTime;
//        }
    }
    return resp;
}

int GeneticAlgorithm::getBestScoreCurrentGeneration(){
    float scoreBest = 0;
    for(int i = 0; i < agentsPool->size();i++){
        int score = agentsPool->at(i)->getTotalScore();
        if(scoreBest <= score){
            scoreBest = score;
        }
    } 
    return scoreBest;
}

int GeneticAlgorithm::getCurrentGeneration(){
    return this->currentGeneration;
}
