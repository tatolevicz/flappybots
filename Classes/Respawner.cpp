//
//  Respawner.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Respawner.hpp"

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

    // Director::getInstance()->setNotificationNode(respawner);
    // Director::getInstance()->getScheduler()->scheduleUpdateForTarget(respawner,0,false);
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
}

void Respawner::stop(){
    shouldRespawn = false;
}

void Respawner::schedule(){
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(Respawner::update, this), this, 1.0f / 60, false, "respawner");
}

void Respawner::unschedule(){
    Director::getInstance()->getScheduler()->unschedule("respawner", this);
}

void Respawner::update(float dt){
    currentTime += dt;

    bool condition_1 = currentTime >= respawnRate;
    bool condition_2 = shouldRespawn;
    bool condition_3 = this->sceneNode;

    if(condition_1 && condition_2 && condition_3){
        log("update called from respawner after %2.1f seconds.",currentTime);
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
}

void Respawner::respawn(){
    auto columnTest = Column::create();
    columnTest->setPosition(Vec2(this->screenSize.width*1.5f, 0));
    columnTest->randomizeHeight();
    sceneNode->addChild(columnTest,10);
}
