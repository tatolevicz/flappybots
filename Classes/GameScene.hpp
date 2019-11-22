//
//  GameScene.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef GameScene_hpp
#define GameScene_hpp
#include "cocos2d.h"
using namespace cocos2d;

class GameScene : public Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void setupScreen(Vec2 origin, Size visibleSize);
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};
#endif