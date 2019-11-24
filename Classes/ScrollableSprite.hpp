//
//  ScrollableSprite.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef ScrollableSprite_hpp
#define ScrollableSprite_hpp

#include "cocos2d.h"
using namespace cocos2d;

class ScrollableSprite : public Sprite
{
    public:
        Size screenSize;
        ScrollableSprite();
        ~ScrollableSprite();
        static ScrollableSprite* create();
        void start();
        void stop();
        void setRate(float newRate);
        void setResetPosition(Vec2 newResetPos);
        
    private:
        void initialSetup();
        void scrollSprite();
        float rate = 1.0f; 
        Vec2 resetPosition = Vec2::ZERO;
        float baseSpeed = 15.0f;
};
#endif 