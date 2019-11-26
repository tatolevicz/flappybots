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
class AgentFlappyBot: Player{
    private:

    public:
        AgentFlappyBot();
        ~AgentFlappyBot();
        AgentFlappyBot* create();
};

#endif /* AgentFlappyBot_hpp */
