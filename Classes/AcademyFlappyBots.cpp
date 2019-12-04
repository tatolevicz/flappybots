//
//  AcademyFlappyBots.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#include "AcademyFlappyBots.hpp"
#include "GameManager.hpp"
#include "json.hpp"

using json = nlohmann::json;

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
    loadBestAgent();
}

void AcademyFlappyBots::initAcademy(){
//    this->initPool();
    this->ga = GeneticAlgorithm::create();
    this->ga->retain();
    this->addAgentsToScene();
    this->ga->setIsInitialized(true);
    this->schedule();
}

void AcademyFlappyBots::addAgentsToScene(){
    for(int i = 0; i< this->ga->agentsPool->size();i++){
        this->scene->addChild(this->ga->agentsPool->at(i));
    }
}

void AcademyFlappyBots::schedule(){
   Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(AcademyFlappyBots::update, this), this, 1.0f / 60, false, "academy");
}

void AcademyFlappyBots::unschedule(){
     Director::getInstance()->getScheduler()->unschedule("academy", this);
}

void AcademyFlappyBots::tempCalculate(){

    for(int i = 0; i< this->ga->agentsPool->size(); i++){
        auto agent = this->ga->agentsPool->at(i);
        if(agent->getIsDead())continue;
        
        auto obs = agent->collectObservations();
        vector<float> out_NN = agent->nn->getOutput(obs);
        agent->action(out_NN.at(0));
    }
}

void AcademyFlappyBots::update(float dt){
    if(GameManager::getInstance()->state != GameManager::PLAYING_STATE) return;
    
    if(!this->ga->getIsInitialized())return;
    
    this->currentTime += dt;

    if(this->currentTime < this->timeToCollect)
    return;

    currentTime = 0;

    auto generationFinished = this->ga->checkGenerationFinished();
    if(generationFinished){
        if(this->ga->checkTrainingFinished()){
            this->scene->gameOver();
            GameManager::getInstance()->state = GameManager::FINISHED_STATE;
            return;
        }
        else{
            this->scene->gameOver();
            this->ga->nextGeneration();
            this->scene->restartGame();
        }        
    }

    this->tempCalculate();
}

void AcademyFlappyBots::loadBestAgent(){

    auto fileUtils = FileUtils::getInstance();
    auto path = fileUtils->getWritablePath() + "/Estudos/TCC/FlappyCocos/bestAgent.txt";
    FILE* f = fopen(path.c_str(), "rb");
    
    if(f != NULL){
        fseek(f,0,SEEK_END);
        int size = ftell(f);
        char* textJson = (char*)malloc(sizeof(char)*size);
        fread(textJson, sizeof(char), size, f);
        // auto j = json::parse(textJson);
        // log("Readed json: %s",j.dump().c_str());
        log(textJson);
    }
    else
    {
        log("there is no agent file to load.");
    }

    

}

void AcademyFlappyBots::saveBestAgent(){

    auto fileUtils = FileUtils::getInstance();
    auto path = fileUtils->getWritablePath() + "/Estudos/TCC/FlappyCocos/bestAgent.txt";
    auto bestAgent = this->ga->getBestAgent();
    json j;
    
    j["numberOfInputs"] = this->numberOfInputs;
    j["numberOfHiddenLayers"] = this->numberOfHiddenLayers;
    j["numberOfOutputs"] = this->numberOfOutputs;

    json hiddenLayersArr = json::array();
    for(int i = 0; i < this->numberOfHiddenLayers; i++){
        hiddenLayersArr.push_back(this->hiddenLayersSize);
    }
    j["hiddenLayersSize"] = hiddenLayersArr;
    j["weights"] = bestAgent->nn->getWeightsAsVector();

    FILE* f;    
    f = fopen(path.c_str(),"wb");
    if(f != NULL)
    {
        fwrite(j.dump().c_str(),j.dump().size(),sizeof(char),f);   
        fclose(f);
    }
    else{
        log("best agent file not created.");
    }
}

void AcademyFlappyBots::stopTraining(){
    GameManager::getInstance()->state = GameManager::FINISHED_STATE;
    this->saveBestAgent();
}

