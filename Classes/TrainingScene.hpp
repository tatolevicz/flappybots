//
//  TrainingScene.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef TrainingScene_hpp
#define TrainingScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Respawner.hpp"
#include "ScrollableSprite.hpp"
#include "GameManager.hpp"
#include "AgentFlappyBot.hpp"

using namespace cocos2d;
using namespace ui;

class TrainingScene : public Scene
{
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        SpriteFrameCache *spriteSheet;
        Respawner* respawner;
        bool onContactBegin(PhysicsContact& contact);
        void onContactSeparate(PhysicsContact& contact);
        CREATE_FUNC(TrainingScene);
    
        //methods
        void startButtonPressed(Ref* pSender);
        // void setupInput();
        // virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
        void gameOver();
        void stopScene();
        void restartGame();

    private:

        Size screenSize;
        //game variables
        ScrollableSprite* sky;
        ScrollableSprite* ground; 
        ScrollableSprite* trees;

        Sprite* groundCollider;
        
        //ui variables
        Menu* startButton;

        void loadSpriteSheet();
        void setupScreen(Vec2 origin);
        void createParallax();
        // void addPlayer();
        void addAcademy();
        void createRespawner();
        void createUI();
        void addPhysicsGround();
        void addPhysicsPlayer();
        void setPhysicsParameters();
        bool checkCollision(Node* nodeA, Node* nodeB);
        int getState();
        bool isPlayerContact(Node* nodeA, Node* nodeB);
        void initIA();
};
#endif
