//
//  ScrollableSprite.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "ScrollableSprite.hpp"

ScrollableSprite::ScrollableSprite(){}
ScrollableSprite::~ScrollableSprite(){}

ScrollableSprite* ScrollableSprite::create(bool shouldRepeate){

    ScrollableSprite *sclbSprite = new ScrollableSprite();
    if(sclbSprite){
        sclbSprite->autorelease();
        sclbSprite->setShouldRepeat(shouldRepeate);
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
    auto move = MoveTo::create(this->baseSpeed/this->rate,this->endPosition);
    auto resetPos = MoveTo::create(0,this->initPosition);

    ActionInterval* action;

    if(this->shouldRepeat){
        action = RepeatForever::create(Sequence::create(move,resetPos,nullptr));
    }
    else{
        action = Sequence::create(move,nullptr);
    }

    this->runAction(RepeatForever::create(action));
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

void ScrollableSprite::setInitPosition(Vec2 newInitPos){
    this->initPosition = newInitPos;
}

void ScrollableSprite::setEndPosition(Vec2 newEndPos){
    this->endPosition = newEndPos;
}

void ScrollableSprite::setShouldRepeat(bool newShouldRepeat){
    this->shouldRepeat = newShouldRepeat;
}

