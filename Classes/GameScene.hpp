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
#include "Respawner.hpp"
#include "ScrollableSprite.hpp"
#include "GameManager.hpp"
#include "Player.hpp"

using namespace cocos2d;
using namespace ui;

class GameScene : public Scene
{
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        SpriteFrameCache *spriteSheet;
        Respawner* respawner;
        bool onContactBegin(PhysicsContact& contact);
        CREATE_FUNC(GameScene);
    
        //methods
        void startButtonPressed(Ref* pSender);
        void setupInput();
        virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);

    private:

        Size screenSize;
        //game variables
        ScrollableSprite* sky;
        ScrollableSprite* ground; 
        ScrollableSprite* trees;

        Player* player;
        Sprite* groundCollider;
        
        //ui variables
        Menu* startButton;

        void loadSpriteSheet();
        void setupScreen(Vec2 origin);
        void createParallax();
        void addPlayer();
        void createRespawner();
        void createUI();
        void addPhysicsGround();
        void addPhysicsPlayer();
        void setPhysicsParameters();
};
#endif
