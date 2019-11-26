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

Vector<float>* AgentFlappyBot::collectObservations(){
    // auto resp = new Vector<float>();
    // //simulating distance x
    // float val1 = ((float) rand()) / (float) RAND_MAX;
    // //simulation distance y
    // float val2 = ((float) rand()) / (float) RAND_MAX;
    // //getting y position
    // float val3 = this->getPosition().y;

    // resp->pushBack(val1);
    // resp->pushBack(val2);
    // resp->pushBack(val3);

    return nullptr;
}

void AgentFlappyBot::action(float value){
    if(value > 0.5f){
        this->jump();
    }
}

// void AgentFlappyBot::setWeights(Vector<float> newWeights){
//     this->weights = newWeights;
// }

// Vector<float> AgentFlappyBot::getWeights(){
//     return this->weights;
// }
     
        


