//
//  AgentFlappyBot.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#include "AgentFlappyBot.hpp"

AgentFlappyBot::AgentFlappyBot(){

}
AgentFlappyBot::~AgentFlappyBot(){}

AgentFlappyBot* AgentFlappyBot::create(){
    log("CREATE AgentFLAPPY");
    AgentFlappyBot* agent = new AgentFlappyBot();
 
    if(agent && agent->initAgent()){
        agent->autorelease();
        return agent;
    }
    CC_SAFE_DELETE(agent);
    return NULL;
}

bool AgentFlappyBot::initAgent(){
    this->init();
    this->initialSetup();
    return true;
}

void AgentFlappyBot::initialSetup(){
  
}


