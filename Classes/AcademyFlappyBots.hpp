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
#include "GeneticAlgorithm.hpp"

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
        TrainingScene* scene;

    GeneticAlgorithm *ga;
    void addAgentsToScene();
    
    public:
        AcademyFlappyBots();
        ~AcademyFlappyBots();
        static AcademyFlappyBots* getInstance();
        float timeToCollect = 0.0;
        float currentTime = 0;
        void setScene(TrainingScene* scene);

        //Neural Networks infos
        int numberOfInputs = 2;
        int numberOfHiddenLayers = 1;
        int numberOfOutputs = 1;
        //TODO:change this to be an vector
        int hiddenLayersSize = 2;

        void saveBestAgent(AgentFlappyBot* agent);
};
#endif /* AcademyFlappyBots_hpp */
