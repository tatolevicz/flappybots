//
//  ScrollableSprite.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "ScrollableSprite.hpp"

ScrollableSprite::ScrollableSprite(){}
ScrollableSprite::~ScrollableSprite(){}

ScrollableSprite* ScrollableSprite::create(){

    ScrollableSprite *sclbSprite = new ScrollableSprite();
    if(sclbSprite){
        sclbSprite->autorelease();
        sclbSprite->initialSetup();
        return sclbSprite;
    }
    CC_SAFE_DELETE(sclbSprite);
    return NULL;
}

void ScrollableSprite::initialSetup(){
    this->screenSize = Director::getInstance()->getVisibleSize();
}

void ScrollableSprite::scrollSprite(){
    auto move = MoveTo::create(this->baseSpeed/this->rate,Vec2(-this->screenSize.width,this->resetPosition.y));
    auto resetPos = MoveTo::create(0,this->resetPosition);
    auto sequence = Sequence::create(move,resetPos,nullptr);
    this->runAction(RepeatForever::create(sequence));
}

void ScrollableSprite::start(){
    this->scrollSprite();
}

void ScrollableSprite::stop(){
    this->stopAllActions();
}

void ScrollableSprite::setRate(float newRate){
    this->rate = newRate;
}

void ScrollableSprite::setResetPosition(Vec2 newResetPos){
    this->resetPosition = newResetPos;
}