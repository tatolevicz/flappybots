//
//  Respawner.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef Respawner_hpp
#define Respawner_hpp

#include "cocos2d.h"
#include "Column.hpp"
using namespace cocos2d;

class Respawner : public Node
{
    public:
        Respawner();
        ~Respawner();
        static Respawner* create();
        
        void setGapSize(float newGapSize);
        void setRespawnRate(float newRespawnRate);
        void setSceneNode(Scene* sceneNodeRef);
        void start();
        void stop();
        void setSpeed(float newSpeed);
        void restart();
    
    private:
        float respawnRate = 1.7f;
        int poolSize = 10;
        float currentTime = 0.0f;
        bool shouldRespawn = false;
        Scene* sceneNode;
        Size screenSize;
        Vector<Column*> *pool = new Vector<Column*>();
        int nextColumnIdx = 0;

        void initializePool();
        void update(float dt);  
        void schedule();
        void unschedule();  
        void respawn();  
        float randomizeHeight();

        //column specifics
        float gapSize = 140.0f;
        float speed =  1.8f;  
        Vec2 getInitialPosition();  
};

#endif 
