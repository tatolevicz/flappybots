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
        virtual ~Column();
        virtual bool initColumn(float gapSize);
        static Column* create(float gapSize);
        void setGapSize(float newGapSize);
    private:
        void initialSetup();
        void addSprites();
        void addPhysics();
        void addPhysicsToNode(Node* node, int tag, const char* name, bool shouldCollide = true);
        float gapSize;
        Sprite* spriteUp;
        Sprite* spriteDown;
        Node* scoreArea;
};

#endif 