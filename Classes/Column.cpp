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
    Column* sprite = new Column();

    if (sprite->initWithSpriteFrameName("Sprites/ColumnSprite.png"))
    {
        sprite->autorelease();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return NULL;
}
