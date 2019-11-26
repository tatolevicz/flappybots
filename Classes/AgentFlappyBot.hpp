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
    public:
        AgentFlappyBot();
        virtual ~AgentFlappyBot();
        static AgentFlappyBot* create();
};

#endif /* AgentFlappyBot_hpp */
