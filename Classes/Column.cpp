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

    auto spriteUp = Sprite::create("Sprites/column.png");
    auto spriteDown = Sprite::create("Sprites/column.png");
    auto scoreArea = Node::create();

     auto scoreAreaHeight = visibleSize.height*0.17;

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

    if (container)
    {
        container->autorelease();
        return container;
    }

    CC_SAFE_DELETE(container);
    return NULL;
}
