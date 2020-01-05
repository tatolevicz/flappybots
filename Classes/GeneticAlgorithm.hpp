//
//  GeneticAlgorithm.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 01/12/19.
//

#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AgentFlappyBot.hpp"

class GeneticAlgorithm : public cocos2d::Ref{
    
private:
    int generationSize = 100;
    int numerOfNegerations = 150;
    int currentGeneration = 1;
    void initPool();
    Vector<AgentFlappyBot*> getBestTwoAgents();
    void setMutation(AgentFlappyBot* bestAgent);
    void copyBestWeights(AgentFlappyBot* fromAgent,AgentFlappyBot* toAgent);
    bool containsIdx(vector<int> vec,int idx);
    bool isInitialized = false;
    
    
public:
    GeneticAlgorithm();
    ~GeneticAlgorithm();
    virtual bool init();
    CREATE_FUNC(GeneticAlgorithm);
    AgentFlappyBot*  permuteGenes(AgentFlappyBot* father, AgentFlappyBot* mother);
    void nextGeneration();
    bool checkGenerationFinished();
    bool checkTrainingFinished();
    Vector<AgentFlappyBot*> *agentsPool = new Vector<AgentFlappyBot*>();
    bool getIsInitialized();
    void setIsInitialized(bool val);
    AgentFlappyBot* getBestAgent();
    float getTimeCurrentGeneration();
    int getBestScoreCurrentGeneration();
    int getCurrentGeneration();
    AgentFlappyBot* bestAgentOfAllTime;
    
};

#endif /* GeneticAlgorithm_hpp */
