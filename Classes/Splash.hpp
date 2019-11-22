//
//  Splash.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//
#ifndef Splash_hpp
#define Splash_hpp

#include "cocos2d.h"
using namespace cocos2d;

class Splash : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void setupScreen(Vec2 origin, Size visibleSize);
    // implement the "static create()" method manually
    CREATE_FUNC(Splash);
};
#endif /* Splash_hpp */