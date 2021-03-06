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
      virtual ~Player();
      /* Initializes an player with animation and initial position. */
      virtual bool init() override;
      static Player* create();
      void jump();
      void addAnimation();
      void stopAnimation();
      virtual void die();
      void reset();

      int getTotalScore();
      float getLifeTime();
      void resetScore();
      void score();
      
      float fixedX = 300;
      
      bool getIsDead();
     
       bool isDead = false;
      
    private:
      Vector<SpriteFrame*> animFrames;
      void initialSetup();
      void addPhysics();
      void flap();
      void schedule();
      void unschedule();
      void update(float dt);
      float lastYPosition;
      float rotationRate = 6.0f;
      float maxAngle = 30.0f;
      void rotate(float dt);
      void checkLimit();

    protected:
      Size screenSize;
      float currentAngle;
      int totalScore = 0;
      float totalLifeTime = 0;
    
};

#endif
