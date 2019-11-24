//
//  Column.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Column.hpp"

USING_NS_CC;

Column::Column() {}

Column::~Column() {}

Column* Column::create()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    Column* container = new Column();
    container->screenSize = visibleSize;

    auto spriteUp = Sprite::create("Sprites/column.png");
    auto spriteDown = Sprite::create("Sprites/column.png");
    auto scoreArea = Node::create();

    auto scoreAreaHeight = visibleSize.height*container->getGapSize();

    spriteUp->setAnchorPoint(Vec2(0.5,0));
    spriteDown->setAnchorPoint(Vec2(0.5,0));
    spriteDown->setRotation(180);

    spriteUp->setPosition(Vec2(0,scoreAreaHeight/2));
    spriteDown->setPosition(Vec2(0,-scoreAreaHeight/2));
    scoreArea->setContentSize(Size(spriteUp->getContentSize().width/2,scoreAreaHeight));
    scoreArea->setAnchorPoint(Vec2(0,0.5));

    container->addChild(spriteUp);
    container->addChild(spriteDown);
    container->addChild(scoreArea);

    container->randomizeHeight();

    if (container)
    {
        container->autorelease();
        return container;
    }

    CC_SAFE_DELETE(container);
    return NULL;
}

void Column::randomizeHeight(){
    auto currentPos = this->getPosition();
    float random = ((float) rand()) / (float) RAND_MAX;
    log("random number %f", random);
    this->setPosition(currentPos.x,this->screenSize.height*random);
}

void Column::setGapSize(float newGapSize){
    this->gapSize = newGapSize;
}

float Column::getGapSize(){
    return this->gapSize;
}