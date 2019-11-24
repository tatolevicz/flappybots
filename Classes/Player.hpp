//
//  Player.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

 class Player : public Sprite{

     public:
        Player();
        ~Player();
        static Player* create();
        void jump();
        void addAnimation();
        void stopAnimation();
     private:
        Size screenSize;
        void initialSetup();
        void addPhysics();
 };

#endif
