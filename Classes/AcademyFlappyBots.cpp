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
        if(inferenceModeOn){
            this->initializeBestWeights(agent); 
        }
        else{
            this->initializeWeights(agent);
            this->initializetLastBestAgent();
            this->setMutation(agent);
        }
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

void AcademyFlappyBots::initializetLastBestAgent(){
    if(!this->lastBestAgent){
        this->lastBestAgent = AgentFlappyBot::create();
        this->lastBestAgent->retain();  
        initializeBestWeights(this->lastBestAgent); 
    }
}


void AcademyFlappyBots::initializeBestWeights(AgentFlappyBot* agent){

    vector<float> resp;

    resp.reserve(12);

    resp.push_back(-0.04);
    resp.push_back(0.09);
    resp.push_back(-0.90);
    resp.push_back(0.49);
    resp.push_back(-0.68);
    resp.push_back(0.58);
    resp.push_back(0.63);
    resp.push_back(-0.75);
    resp.push_back(0.61);
    resp.push_back(-0.58);
    resp.push_back(0.47);
    resp.push_back(-0.50);

    agent->setWeights(resp);

    // if(!this->lastBestAgent){
    //     this->lastBestAgent = AgentFlappyBot::create();
    //     this->lastBestAgent->retain();
    //     this->initializeWeights(this->lastBestAgent);
    // }
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
// ################ 12 weights
        auto weights = agent->getWeights();

        float out1 = obs.at(0)*weights.at(0);
        float out2 = obs.at(1)*weights.at(1); 

        float out3 = obs.at(0)*weights.at(2);
        float out4 = obs.at(1)*weights.at(3);

        float out5 = obs.at(0)*weights.at(4);  
        float out6 = obs.at(1)*weights.at(5);

        float out7 = obs.at(0)*weights.at(6);  
        float out8 = obs.at(1)*weights.at(7);

        float out9 = this->activationFunction(out1 + out2);
        float out10 = this->activationFunction(out3 + out4);  
        float out11 = this->activationFunction(out5 + out6);
        float out12 = this->activationFunction(out7 + out8);

        float out = this->activationFunction(out9*weights.at(8) + out10*weights.at(9) + out11*weights.at(10) + + out12*weights.at(11));

// ################ 12 weights 

        // auto weights = agent->getWeights();

        // float out1 = obs.at(0)*weights.at(0);
        // float out2 = obs.at(1)*weights.at(1);  
        // float out3 = obs.at(2)*weights.at(2);

        // float out4 = obs.at(0)*weights.at(3);
        // float out5 = obs.at(1)*weights.at(4);  
        // float out6 = obs.at(2)*weights.at(5);

        // float out7 = obs.at(0)*weights.at(6);
        // float out8 = obs.at(1)*weights.at(7); 
        // float out9 = obs.at(2)*weights.at(8);

        // float out10 = obs.at(0)*weights.at(9);
        // float out11 = obs.at(1)*weights.at(10);  
        // float out12 = obs.at(2)*weights.at(11);

        // float out13 = this->activationFunction((out1 + out2 + out3));
        // float out14 = this->activationFunction((out4 + out5 + out6));
        // float out15 = this->activationFunction((out7 + out8 + out9));
        // float out16 = this->activationFunction((out10 + out11 + out12));

        // float out17 = this->activationFunction(out13*weights.at(12) + out14*weights.at(13) + out15*weights.at(14) + out16*weights.at(15));
        // float out18 = this->activationFunction(out13*weights.at(16) + out14*weights.at(17) + out15*weights.at(18) + out16*weights.at(19));
        // float out19 = this->activationFunction(out13*weights.at(20) + out14*weights.at(21) + out15*weights.at(22) + out16*weights.at(23));

        // float out = this->activationFunction((out17*weights.at(24) + out18*weights.at(25) + out18*weights.at(26)));

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

bool AcademyFlappyBots::containsIdx(vector<int> vec,int idx){
    for(int i = 0; i < vec.size(); i++){
        if(vec.at(i) == idx){
            return true;
        }
    }
    return false;
}

void AcademyFlappyBots::setMutation(AgentFlappyBot* agent){
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
        newWeight *= this->weightMagnitude;
        if(isNegative){
            newWeight *= -1;
        }
        weights.at(randVec.at(i)) = newWeight;
    }
    
    agent->setWeights(weights);
}

void AcademyFlappyBots::nextGeneration(){
    
    // auto bestAgent = this->getBestAgent();
    // AgentFlappyBot* sonAgent = this->permuteGenes(bestAgent,this->lastBestAgent);

    // //save score from de best agent of this generation
    // // if(this->lastBestAgent->getTotalScore() <= bestAgent->getTotalScore()){
    // //     this->lastBestAgent->setTotalScore(bestAgent->getTotalScore());
    // // }
    
    // //save weights from de best agent of this generation
    // if(this->lastBestAgent->getTotalScore() <= bestAgent->getTotalScore()){
    //     this->lastBestAgent->setWeights(bestAgent->getWeights());
    //     this->lastBestAgent->setTotalScore(bestAgent->getTotalScore());
    //     auto weights = bestAgent->getWeights();
    //     log("Best Agent:");
    //     for(int i = 0; i < bestAgent->numberOfWeights; i++){
    //         log("%3.2f",weights.at(i));
    //     }
    //     log("Score: %d",bestAgent->getTotalScore());
    // }
    
    // for(int i = 0; i< this->agentsPool->size(); i++){
    //     auto agent = this->agentsPool->at(i);
    //     agent->reset();
    //     this->copyBestWeights(sonAgent,agent);
    // }

    // // to preserve some percent agent with the bests weights from last generation
    // int remainAmount = (int)floor(this->generationSize*0.05);
    // // log("Remaning: %d",remainAmount);

    // for(int i = 0; i< this->agentsPool->size(); i++){
    //     auto agent = this->agentsPool->at(i);
    //     if(i >= remainAmount){
    //         this->setMutation(agent);
    //     }
    //     else{
    //         agent->setWeights(bestAgent->getWeights());
    //     }
    // }

    //########## teste
    auto bestAgents = this->getBestTwoAgents();
    AgentFlappyBot* sonAgent = this->permuteGenes(bestAgents.at(0),bestAgents.at(1));

    //save score from de best agent of this generation
    // if(this->lastBestAgent->getTotalScore() <= bestAgent->getTotalScore()){
    //     this->lastBestAgent->setTotalScore(bestAgent->getTotalScore());
    // }
    
    //save weights from de best agent of this generation
    auto bestAgent = bestAgents.at(0);

    if(this->lastBestAgent->getTotalScore() < bestAgent->getTotalScore()){
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

    // to preserve some percent agent with the bests weights from last generation
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

    //###########
}

Vector<AgentFlappyBot*> AcademyFlappyBots::getBestTwoAgents(){
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

    // if(this->lastBestAgent->getTotalScore() > secondBestScore && 
    // this->lastBestAgent->getTotalScore() > firstBestScore){
    //     secondAgent->setWeights(this->lastBestAgent->getWeights());
    // }
    
    agents.pushBack(firstAgent);
    agents.pushBack(secondAgent);
    return agents;
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
            if(!inferenceModeOn){
                this->nextGeneration();
            }
            else{
                for(int i = 0; i< this->agentsPool->size(); i++){
                    auto agent = this->agentsPool->at(i);
                    agent->reset();
                }
            }
            this->scene->restartGame();
        }        
    }

    this->tempCalculate();
}

float AcademyFlappyBots::activationFunction(float neuron){
    float resp = 1/(1 + expf(-neuron));
    return resp;
}


