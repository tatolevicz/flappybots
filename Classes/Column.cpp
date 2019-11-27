//
//  Column.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Column.hpp"
#include "GameManager.hpp"

USING_NS_CC;

Column::Column() {}

Column::~Column() {
    this->unschedule();
}

Column* Column::create(float gapSize = 17.0f)
{
    Column* container = new Column();

    if (container && container->initColumn(gapSize))
    {
        container->autorelease();
        return container;
    }

    CC_SAFE_DELETE(container);
    return NULL;
}

void Column::initialSetup(){
    this->init();
    this->addSprites();
    this->addPhysics();
    this->schedule();
}

bool Column::initColumn(float gapSize){
    this->setGapSize(gapSize);
    this->initialSetup();
    return true;
}

void Column::addSprites(){
    this->screenSize = Director::getInstance()->getVisibleSize();;
    this->spriteUp = Sprite::create("Sprites/column.png");
    this->spriteDown = Sprite::create("Sprites/column.png");
    this->scoreArea = Node::create();

    auto scoreAreaHeight = this->gapSize;

    spriteUp->setAnchorPoint(Vec2(0.5,0));
    spriteDown->setAnchorPoint(Vec2(0.5,0));
    spriteDown->setRotation(180);

    spriteUp->setPosition(Vec2(0,scoreAreaHeight/2));
    spriteDown->setPosition(Vec2(0,-scoreAreaHeight/2));
    scoreArea->setContentSize(Size(spriteUp->getContentSize().width/2,scoreAreaHeight));
    scoreArea->setAnchorPoint(Vec2(0,0.5));

    this->addChild(spriteUp);
    this->addChild(spriteDown);
    this->addChild(scoreArea);
}

void Column::addPhysics(){
    this->addPhysicsToNode( this->spriteUp,
                            GameManager::getInstance()->column_tag,
                            GameManager::getInstance()->column_name
                            );
    this->addPhysicsToNode( this->spriteDown,
                            GameManager::getInstance()->column_tag,
                            GameManager::getInstance()->column_name
                            );
    this->addPhysicsToNode( this->scoreArea,
                            GameManager::getInstance()->scoreArea_tag,
                            GameManager::getInstance()->scoreArea_name,
                            false
                            );
}

void Column::addPhysicsToNode(Node* node, int tag,const char* name, bool shouldCollide){
    auto body = PhysicsBody::createBox(node->getContentSize(),PHYSICSBODY_MATERIAL_DEFAULT,Vec2::ZERO);
    body->setDynamic(false);
    body->setCategoryBitmask(GameManager::getInstance()->obstacle_bit_mask_category);
    if(shouldCollide){
        body->setCollisionBitmask(GameManager::getInstance()->player_bit_mask_category);
    }
    else{
        body->setCollisionBitmask(0);
    }
    body->setContactTestBitmask(GameManager::getInstance()->player_bit_mask_category);
    node->setName(name);
    node->setTag(tag);
    node->addComponent(body);
}

void Column::setGapSize(float newGapSize){
    this->gapSize = newGapSize;
}

void Column::setInitPosition(Vec2 initPos){
    this->initPosition = initPos;
}
void Column::setEndPosition(Vec2 endPos){
    this->endPosition = endPos;
}
void Column::setRate(float newRate){
    this->rate = newRate;
}
void Column::start(){
    //call movement
    this->shouldMove = true;
}
void Column::stop(){
    //stop movement
    this->shouldMove = false;
}

void Column::schedule(){
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(Column::update, this), this, 1.0f / 60, false, "column");
}

void Column::unschedule(){
    Director::getInstance()->getScheduler()->unschedule("column", this);
}

void Column::update(float dt){
    this->move(dt);
}

void Column::move(float dt){
    if(!shouldMove)return;
    auto currentPos = this->getPosition();
    auto newPosition = Vec2(currentPos.x - dt*GameManager::getInstance()->speedObstacle*GameManager::getInstance()->worldSpeed,currentPos.y);
    if(newPosition.x < endPosition.x){
        this->shouldMove = false;
        return;
    }
    this->setPosition(newPosition); 
}
