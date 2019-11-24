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

        //ui variables
        Menu* startButton;
        //methods
        void startButtonPressed(Ref* pSender);

    private:
        void loadSpriteSheet();
        void setupScreen(Vec2 origin, Size visibleSize);
        void createParallax(Size visibleSize);
        void addPlayer(Size visibleSize);
        void createRespawner();
        void createUI();
};
#endif
