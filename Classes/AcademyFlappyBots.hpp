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

        TrainingScene* scene;

    public:
        AcademyFlappyBots();
        ~AcademyFlappyBots();
        static AcademyFlappyBots* getInstance();
        Vector<AgentFlappyBot*> *agentsPool = new Vector<AgentFlappyBot*>();
        int generationSize = 1;
        int numerOfNegerations = 20;
        int currentGeneration = 1;

        void setScene(TrainingScene* scene);
};
#endif /* AcademyFlappyBots_hpp */
