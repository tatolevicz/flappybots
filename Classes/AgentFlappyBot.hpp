//
//  AgentFlappyBot.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#ifndef AgentFlappyBot_hpp
#define AgentFlappyBot_hpp

#include <stdio.h>
#include "Player.hpp"

using namespace std;
class AgentFlappyBot: public Player{
    private:
        void initialSetup();
        std::vector<float> weights;
        float getObstacleDistance();
    public:
        virtual void die() override;
        AgentFlappyBot();
        virtual ~AgentFlappyBot();
        virtual bool initAgent();
        static AgentFlappyBot* create();
        vector<float> collectObservations();
        void action(float value = 1.0f);
        void setWeights(vector<float> newWeights);
        vector<float> getWeights();
        DrawNode* drawNode = nullptr;
        void clearDrawnNode();
        PhysicsRayCastInfo applyRayCast(Vec2 direction, float distance);
        Vec2 observe();
        int numberOfWeights = 9;
        
};

#endif /* AgentFlappyBot_hpp */
