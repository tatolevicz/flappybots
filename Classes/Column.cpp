//
//  Column.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Column.hpp"
#include "Gamemanager.hpp"

USING_NS_CC;

Column::Column() {}

Column::~Column() {}

Column* Column::create(float _gapSize = 17.0f)
{
    Column* container = new Column();

    if (container)
    {
        container->autorelease();
        container->setGapSize(_gapSize);
        container->initialSetup();
        return container;
    }

    CC_SAFE_DELETE(container);
    return NULL;
}

void Column::initialSetup(){
    this->addSprites();
    this->addPhysics();
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
    this->addPhysicsToNode(this->spriteUp);
    this->addPhysicsToNode(this->spriteDown);
    this->addPhysicsToNode(this->scoreArea,false);
}

void Column::addPhysicsToNode(Node* node,bool shouldCollide){
    auto body = PhysicsBody::createBox(node->getContentSize(),PHYSICSBODY_MATERIAL_DEFAULT,Vec2::ZERO);
    body->setDynamic(false);
    body->setCategoryBitmask(GameManager::getInstance()->obstacle_bit_mask_category);
    if(shouldCollide){
        body->setCollisionBitmask(GameManager::getInstance()->player_bit_mask_category);
    }
    body->setContactTestBitmask(GameManager::getInstance()->player_bit_mask_category);

    node->addComponent(body);
}

void Column::setGapSize(float newGapSize){
    this->gapSize = newGapSize;
}

