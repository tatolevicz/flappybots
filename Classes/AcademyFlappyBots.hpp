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
#include "json.hpp"

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
        void infere();
        TrainingScene* scene;

        GeneticAlgorithm *ga;
    
    public:
        AcademyFlappyBots();
        ~AcademyFlappyBots();
        static AcademyFlappyBots* getInstance();
        float timeToCollect = 0.0;
        float currentTime = 0;
        void setScene(TrainingScene* scene);
        void addAgentsToScene();
        void removeAgentsFromScene();

        //Neural Networks infos
        int numberOfInputs = 2;
        int numberOfHiddenLayers = 1;
        int numberOfOutputs = 1;
        //TODO:change this to be an vector
        int hiddenLayersSize = 4;

        void saveBestAgent();
        nlohmann::json loadBestAgent();
        void stopTraining();
        void startTraining();
        void startInference();
        AgentFlappyBot* inferenceBird;
        bool inferenceModeOn = false;

        int getCurrentGeneration();
        float cumulatedReward = 0;
};
#endif /* AcademyFlappyBots_hpp */
