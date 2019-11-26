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
    public:
        AgentFlappyBot();
        virtual ~AgentFlappyBot();
        virtual bool initAgent();
        static AgentFlappyBot* create();
        vector<float> collectObservations();
        void action(float value = 1.0f);
        void setWeights(vector<float> newWeights);
        vector<float> getWeights();
        
};

#endif /* AgentFlappyBot_hpp */
