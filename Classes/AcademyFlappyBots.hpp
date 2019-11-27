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
        void copyBestWeights(AgentFlappyBot* bestAgent,AgentFlappyBot* agent);
        void setMutation(AgentFlappyBot* bestAgent);

        void nextGeneration();
        void initializeWeights(AgentFlappyBot* agent);

        float weightMagnitude = 1.0f;

        TrainingScene* scene;

    public:
        AcademyFlappyBots();
        ~AcademyFlappyBots();
        static AcademyFlappyBots* getInstance();
        Vector<AgentFlappyBot*> *agentsPool = new Vector<AgentFlappyBot*>();
        int generationSize = 100;
        int numerOfNegerations = 1000;
        int currentGeneration = 1;
        float timeToCollect = 0.0;
        float currentTime = 0;
        void setScene(TrainingScene* scene);
        float activationFunction(float neuron);
        AgentFlappyBot* lastBestAgent = nullptr;
        AgentFlappyBot*  permuteGenes(AgentFlappyBot* father, AgentFlappyBot* mother);
};
#endif /* AcademyFlappyBots_hpp */
