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

class Column : public Sprite
{
public:
    Size screenSize;
    Column();
    virtual ~Column();
    virtual bool initColumn(float gapSize);
    static Column* create(float gapSize);
    void setGapSize(float newGapSize);
    void setInitPosition(Vec2 initPos);
    void setEndPosition(Vec2 endPos);
    void setRate(float);
    void start();
    void stop();
    Vec2 initPosition;
    Vec2 endPosition;
    float rate;
    bool shouldMove = false;
    
    
private:
    void initialSetup();
    void addSprites();
    void addPhysics();
    void addPhysicsToNode(Node* node, int tag, const char* name, bool shouldCollide = true);
    float gapSize;
    Sprite* spriteUp;
    Sprite* spriteDown;
    Node* scoreArea;
    void schedule();
    void unschedule();
    void update(float dt);
    void move(float dt);
};

#endif 
