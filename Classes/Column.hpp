//
//  Column.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef Column_hpp
#define Column_hpp

#include "cocos2d.h"
#include "ScrollableSprite.hpp"
using namespace cocos2d;

class Column : public ScrollableSprite
{
    public:
        Size screenSize;
        Column();
        ~Column();
        static Column* create(float _gapSize);
        void setGapSize(float newGapSize);
    private:
        void initialSetup();
        void addSprites();
        void addPhysics();
        void addPhysicsToNode(Node* node,bool shouldCollide = true);
        float gapSize;
        Sprite* spriteUp;
        Sprite* spriteDown;
        Node* scoreArea;
};

#endif 