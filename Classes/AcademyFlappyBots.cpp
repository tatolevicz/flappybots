//
//  AcademyFlappyBots.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#include "AcademyFlappyBots.hpp"
#include "GameManager.hpp"

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
}

void AcademyFlappyBots::initAcademy(){
    this->ga = GeneticAlgorithm::create();
    this->ga->retain();
    this->ga->setIsInitialized(true);
    this->schedule();
}

void AcademyFlappyBots::addAgentsToScene(){
    for(int i = 0; i< this->ga->agentsPool->size();i++){
        this->scene->addChild(this->ga->agentsPool->at(i));
    }
}

void AcademyFlappyBots::removeAgentsFromScene(){
    for(int i = 0; i< this->ga->agentsPool->size();i++){
        this->scene->removeChild(this->ga->agentsPool->at(i),false);
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

void AcademyFlappyBots::infere(){
    auto obs = this->inferenceBird->collectObservations();
    vector<float> out_NN = this->inferenceBird->nn->getOutput(obs);
    this->inferenceBird->action(out_NN.at(0));
}

void AcademyFlappyBots::update(float dt){
    if(GameManager::getInstance()->state != GameManager::PLAYING_STATE) return;
    
    if(!this->ga->getIsInitialized())return;

    if(inferenceModeOn){
        
        if(this->inferenceBird->getIsDead())
        {
            this->scene->gameOver();
        }
        
        this->infere();
        return;
    }
    
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

json AcademyFlappyBots::loadBestAgent(){

    auto fileUtils = FileUtils::getInstance();
    auto path = fileUtils->getWritablePath() + "/Estudos/TCC/FlappyCocos/bestAgent.json";
    FILE* f = fopen(path.c_str(), "rb");
    
    if(f != NULL){
        fseek(f,0,SEEK_END);
        int size = ftell(f);
        rewind (f);
        char* textJson = (char*)malloc(sizeof(char)*size*2);
        memset(textJson, 0, size*2);
        fread(textJson, sizeof(char), size, f);
        json j = json::parse(textJson);
        fclose (f);
        free(textJson);
        return j;
    }
    else
    {
        log("there is no agent file to load.");
    }
    return nullptr;
}

void AcademyFlappyBots::saveBestAgent(){

    auto fileUtils = FileUtils::getInstance();
    auto path = fileUtils->getWritablePath() + "/Estudos/TCC/FlappyCocos/bestAgent.json";
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
    remove(path.c_str());
    f = fopen(path.c_str(),"wb");
    if(f != NULL)
    {
        string txtFile = j.dump();
        fwrite(txtFile.c_str(),txtFile.length(),sizeof(char),f);
        fclose(f);
    }
    else{
        log("best agent file not created.");
    }
}

void AcademyFlappyBots::stopTraining(){
    if(inferenceModeOn){
        inferenceModeOn = false;
        this->inferenceBird->removeFromParentAndCleanup(true);
        this->inferenceBird = nullptr;
        return;
    }
    this->removeAgentsFromScene();
    this->saveBestAgent();
}

void AcademyFlappyBots::startTraining(){
    this->addAgentsToScene();
}

void AcademyFlappyBots::startInference(){
    auto j = loadBestAgent();

    if(j.is_null()) return;
    
    this->numberOfInputs = j["numberOfInputs"];
    this->numberOfHiddenLayers = j["numberOfHiddenLayers"];
    this->numberOfOutputs = j["numberOfOutputs"];

    auto hiddenLayersArr = j["hiddenLayersSize"];
    for(int i = 0; i < hiddenLayersArr.size(); i++){
        this->hiddenLayersSize = hiddenLayersArr.at(i);
    }
    auto weights = j["weights"];
    
    this->inferenceBird = AgentFlappyBot::create();
    this->inferenceBird->setWeights(weights);
    this->inferenceBird->stopAnimation();
    this->inferenceBird->getPhysicsBody()->setGravityEnable(true);
    this->scene->addChild(this->inferenceBird);
    inferenceModeOn = true;
}

