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
        virtual ~ScrollableSprite();
        virtual bool initScrollableSprite(bool shouldRepeat);
        static ScrollableSprite* create(bool _shouldRepeat = true);
        void start();
        void stop();
        void setRate(float newRate);
        void setInitPosition(Vec2 newInitPos);
        void setEndPosition(Vec2 newEndPos);
        void setShouldRepeat(bool newShouldRepeat);
        
    private:
        void initialSetup();
        void scrollSprite();
        float rate = 1.0f; 
        Vec2 initPosition = Vec2::ZERO;
        Vec2 endPosition = Vec2::ZERO;
        float baseSpeed = 10.0f;
        bool shouldRepeat = true;
};
#endif 
