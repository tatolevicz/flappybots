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
        this->setMutation(agent);
    }
}

void GeneticAlgorithm::setMutation(AgentFlappyBot* agent){
    auto weights = agent->getWeights();
    int numberOfMutations = agent->numberOfWeights*0.2;
    vector<int> randVec;
    randVec.reserve(numberOfMutations);
    
    int j = 0;
    while(j < numberOfMutations){
        int randIdx = rand() % agent->numberOfWeights;
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

void GeneticAlgorithm::copyBestWeights(AgentFlappyBot* fromAgent,AgentFlappyBot* toAgent){
    auto weights = fromAgent->getWeights();
    toAgent->setWeights(weights);
}
    
void GeneticAlgorithm::nextGeneration(){
    
    auto bestAgents = this->getBestTwoAgents();
    AgentFlappyBot* sonAgent = this->permuteGenes(bestAgents.at(0),bestAgents.at(1));
    for(int i = 0; i< this->agentsPool->size(); i++){
       auto agent = this->agentsPool->at(i);
       agent->reset();
       this->copyBestWeights(sonAgent,agent);
    }

    //################
    // to preserve some percent agent with the bests weights from last generation
    int remainAmount = (int)floor(this->generationSize*0.05);
    // log("Remaning: %d",remainAmount);
    for(int i = 0; i< this->agentsPool->size(); i++){
       auto agent = this->agentsPool->at(i);
       if(i >= remainAmount){
           this->setMutation(agent);
       }
       else{
           agent->setWeights(bestAgents.at((i%2==0?0:1))->getWeights());
       }
       agent->nn->setWeightsFromVector(agent->getWeights());
    }
    //################
}

Vector<AgentFlappyBot*> GeneticAlgorithm::getBestTwoAgents(){
    int firstBestScore = 0;
    int secondBestScore = 0;

    Vector<AgentFlappyBot*> agents;
    agents.reserve(2);

    AgentFlappyBot* firstAgent;
    AgentFlappyBot* secondAgent;

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        auto agentScore = agent->getTotalScore();
        if(agentScore >= firstBestScore){
            firstBestScore = agentScore;
            firstAgent = agent;
        }
    }

    auto firstWeights = firstAgent->getWeights();
    firstAgent->setWeights(firstWeights);

    for(int i = 0; i< this->agentsPool->size(); i++){
        auto agent = this->agentsPool->at(i);
        auto agentScore = agent->getTotalScore();
        if(agentScore >= secondBestScore && agent != firstAgent){
            secondBestScore = agentScore;
            secondAgent = agent;
        }
    }
    
    auto secondWeights = secondAgent->getWeights();
    secondAgent->setWeights(secondWeights);
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
    this->currentGeneration += 1;
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
