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
    
    private:
        float respawnRate = 2.0f;
        float gapSize = 17.0f;
        int poolSize = 5;
        float currentTime = 0.0f;
        bool shouldRespawn = false;
        Scene* sceneNode;
        Size screenSize;

        void update(float dt);  
        void schedule();
        void unschedule();  
        void respawn();       
};

#endif 
