//
//  Respawner.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Respawner.hpp"
#include "GameManager.hpp"

USING_NS_CC;

Respawner::Respawner() {
}

Respawner::~Respawner() {
    this->unschedule();
}

Respawner* Respawner::create()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    Respawner* respawner = new Respawner();
    respawner->screenSize = visibleSize;

    respawner->schedule();

    if (respawner)
    {
        respawner->autorelease();
        return respawner;
    }

    CC_SAFE_DELETE(respawner);
    return NULL;
}

void Respawner::setGapSize(float newGapSize){
    this->gapSize = newGapSize;
}

void Respawner::start(){
    shouldRespawn = true;
    currentTime = respawnRate;
}

void Respawner::stop(){
    shouldRespawn = false;
}

void Respawner::schedule(){
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(Respawner::update, this), this, 1.0f / 60, false, "respawner");
    this->respawnRate /= GameManager::getInstance()->worldSpeed;
}

void Respawner::unschedule(){
    Director::getInstance()->getScheduler()->unschedule("respawner", this);
}

void Respawner::initializePool(){
    pool->reserve(poolSize);
    auto initPos = this->getInitialPosition();
    for(int i = 0; i < poolSize; i++){
        auto col = Column::create(this->gapSize);
        col->setPosition(initPos);
        pool->pushBack(col);
        sceneNode->addChild(col,1);
    }
}

void Respawner::update(float dt){
    currentTime += dt;

    bool condition_1 = currentTime >= respawnRate;
    bool condition_2 = shouldRespawn;
    bool condition_3 = this->sceneNode;

    if(condition_1 && condition_2 && condition_3){
        // log("update called from respawner after %2.1f seconds.",currentTime);
        currentTime = 0.0f;
        this->respawn();
        return;
    }

    if( condition_1 && !condition_3){
        log("There is no scene to respawn please call setSceneNode before start respawning.");
        currentTime = 0.0f;
    }
}

void Respawner::setSceneNode(Scene* sceneNodeRef){
    this->sceneNode = sceneNodeRef;
    if(this->sceneNode){
        this->initializePool();
    }
    else{
        log("Respawner: Problems initializing scenenode. So the pool was not initalized.");
    }
}

void Respawner::respawn(){
    auto column = this->pool->at(nextColumnIdx);
    column->stop();

    float randomY = this->randomizeHeight();
    auto initPos = Vec2(this->screenSize.width + this->screenSize.width/2 , randomY);
    auto endPos = Vec2(-this->screenSize.width/2, randomY);

    column->setPosition(initPos);
    column->setInitPosition(initPos);
    column->setEndPosition(endPos);

    column->setRate(this->speed/2.0f);
    column->start();
    
    nextColumnIdx++;
    if(nextColumnIdx >= poolSize){
        nextColumnIdx = 0;
    }
}

float Respawner::randomizeHeight(){
    float random = ((float) rand()) / (float) RAND_MAX;
    float result = (this->screenSize.height*0.60)*random + this->screenSize.height*0.25;
    // log("random result %f", result);
    return result;
}

void Respawner::setSpeed( float newSpeed){
    this->speed = newSpeed;
}

void Respawner::restart(){
    if(pool->size() < poolSize){
        log("pool probrably not initialized. Can't restart.");
        return;
    }
    auto initPos = this->getInitialPosition();
    for(int i = 0; i < poolSize; i++){
        auto col = pool->at(i);
        col->setPosition(initPos);
        col->stop();
    }
    if(!this->shouldRespawn){
        this->start();
    }
}

Vec2 Respawner::getInitialPosition(){
    return Vec2(this->screenSize.width + this->screenSize.width/2 , this->screenSize.height/2);
}
