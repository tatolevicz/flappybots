//
//  AcademyFlappyBots.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#ifndef AcademyFlappyBots_hpp
#define AcademyFlappyBots_hpp

#include <stdio.h>
#include "AgentFlappyBot.hpp"
#include "TrainingScene.hpp"
#include "NeuralNetwork.hpp"

USING_NS_CC;

class AcademyFlappyBots{
    private:
        static AcademyFlappyBots* instance;
        void initAcademy();
        void initPool();

        void schedule();
        void unschedule();
        void update(float dt);

        void tempCalculate();

        bool checkGenerationFinished();
        bool checkTrainingFinished();
        AgentFlappyBot* getBestAgent();
        Vector<AgentFlappyBot*> getBestTwoAgents();
        void copyBestWeights(AgentFlappyBot* bestAgent,AgentFlappyBot* agent);
        void setMutation(AgentFlappyBot* bestAgent);

        void nextGeneration();
        void initializeWeights(AgentFlappyBot* agent);
        void initializeBestWeights(AgentFlappyBot* agent);

        void initializetLastBestAgent();

        float weightMagnitude = 1.0f;
        TrainingScene* scene;

        //if false need to have at least 2 as generationSize
        bool inferenceModeOn = false;
        NeuralNetwork* nn;
        void initIA();

    public:
        AcademyFlappyBots();
        ~AcademyFlappyBots();
        static AcademyFlappyBots* getInstance();
        Vector<AgentFlappyBot*> *agentsPool = new Vector<AgentFlappyBot*>();
        int generationSize = 100;
        int numerOfNegerations = 100000;
        int currentGeneration = 1;
        float timeToCollect = 0.0;
        float currentTime = 0;
        void setScene(TrainingScene* scene);
        float activationFunction(float neuron);
        AgentFlappyBot* lastBestAgent = nullptr;
        AgentFlappyBot*  permuteGenes(AgentFlappyBot* father, AgentFlappyBot* mother);
        bool containsIdx(vector<int> vec,int idx);
};
#endif /* AcademyFlappyBots_hpp */
