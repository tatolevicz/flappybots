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
      void die();
      void reset();
      
     private:
      Vector<SpriteFrame*> animFrames;
      Size screenSize;
      void initialSetup();
      void addPhysics();
      void flap();
      bool isDead = false;
      void schedule();
      void unschedule();
      void update(float dt);
      float lastYPosition;
      float currentAngle;
      float rotationRate = 6.0f;
      float maxAngle = 30.0f;
};

#endif
