//
//  Splash.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//
#ifndef Splash_hpp
#define Splash_hpp

#include "cocos2d.h"

class Splash : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void setupScreen(cocos2d::Vec2 origin, cocos2d::Size visibleSize);
    // implement the "static create()" method manually
    CREATE_FUNC(Splash);
};
#endif /* Splash_hpp */