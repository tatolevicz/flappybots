//
//  GameScene.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef GameScene_hpp
#define GameScene_hpp
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;

class GameScene : public Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void setupScreen(Vec2 origin, Size visibleSize);
    // implement the "static create()" method manually
    void scrollSprite(Sprite* sprite, 
                        float rate, 
                        Size visibleSize,
                        Vec2 resetPosition = Vec2::ZERO,
                        float baseSpeed = 15.0);
    bool onContactBegin(PhysicsContact& contact);
    CREATE_FUNC(GameScene);
};
#endif