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
    this->screenSize = Director::getInstance()->getVisibleSize();;
    auto spriteUp = Sprite::create("Sprites/column.png");
    auto spriteDown = Sprite::create("Sprites/column.png");
    auto scoreArea = Node::create();
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

void Column::setGapSize(float newGapSize){
    this->gapSize = newGapSize;
}