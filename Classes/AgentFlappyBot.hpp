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

class AgentFlappyBot: public Player{
    private:
        void initialSetup();
        // Vector<float> weights;
    public:
        AgentFlappyBot();
        virtual ~AgentFlappyBot();
        virtual bool initAgent();
        static AgentFlappyBot* create();
        Vector<float>* collectObservations();
        void action(float value = 1.0f);
        // void setWeights(Vector<float> newWeights);
        // Vector<float> getWeights();
        
};

#endif /* AgentFlappyBot_hpp */
