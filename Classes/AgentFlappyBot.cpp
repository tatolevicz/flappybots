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

    this->observe();

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

void AgentFlappyBot::observe(){
    int dist = 700;
    auto direction = Vec2(1,0);
    auto d = direction*dist;
    Vec2 centerSprite = this->getPosition();
    Vec2 point2 = centerSprite + d;
    
    if (this->drawNode)
    {
        this->drawNode->removeFromParentAndCleanup(this->drawNode);
    }
    this->drawNode = DrawNode::create();

    Vec2 points[2];

    int num = 0;
    auto func = [&points, &num](PhysicsWorld& world,
        const PhysicsRayCastInfo& info, void* data)->bool
    {
        if (num < 2)
        {
            points[num++] = info.contact;
            if(num > 1){
                if(info.shape != nullptr){
                    log("Seeing someone called: %s", info.shape->getBody()->getNode()->getName().c_str());   
                }
            }
        }
        return true;
    };

    // for(int i = 0; i< 2; i++){

    // }

    //cheeting here just a little
    auto scene = Director::getInstance()->getRunningScene();
    scene->getPhysicsWorld()->rayCast(func,centerSprite, point2,nullptr);

    this->drawNode->drawSegment(centerSprite, point2,1,Color4F::RED);

    for(int i = 0; i < num;i++){
        this->drawNode->drawDot(points[i],3,Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    }

    scene->addChild(this->drawNode,10);
    
}

void AgentFlappyBot::action(float value){
    // log("Action: %3.2f",value);
    if(value > 0.5f){
        this->jump();
    }
}

void AgentFlappyBot::setWeights(vector<float> newWeights){
    this->weights = newWeights;
    // log("SetWeights: %3.2f  %3.2f  %3.2f",this->weights.at(0),this->weights.at(1),this->weights.at(2));
}

vector<float>  AgentFlappyBot::getWeights(){
    return this->weights;
}
     
        


