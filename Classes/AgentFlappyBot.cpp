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

vector<float> AgentFlappyBot::collectObservations(){
    vector<float> resp;
    resp.reserve(3);
    //simulating distance x
    float val1 = ((float) rand()) / (float) RAND_MAX;
    //simulation distance y
    float val2 = ((float) rand()) / (float) RAND_MAX;
    //getting y position
    float val3 = this->getPosition().y/this->screenSize.height;

    resp.push_back(val1);
    resp.push_back(val2);
    resp.push_back(val3);

    return resp;
}

void AgentFlappyBot::action(float value){
    log("Action: %3.2f",value);
    if(value > 0.5f){
        this->jump();
    }
}

void AgentFlappyBot::setWeights(vector<float> newWeights){
    this->weights = newWeights;
    log("SetWeights: %3.2f  %3.2f  %3.2f",this->weights.at(0),this->weights.at(1),this->weights.at(2));
}

vector<float>  AgentFlappyBot::getWeights(){
    return this->weights;
}
     
        


