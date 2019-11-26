//
//  AgentFlappyBot.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#include "AgentFlappyBot.hpp"
#include "GameManager.hpp"

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
    
    if (this->drawNode)
    {
        this->drawNode->removeFromParentAndCleanup(this->drawNode);
    }
    this->drawNode = DrawNode::create();

    float dist = this->screenSize.width - this->getPosition().x*1.5f;

    vector<float> directionsY;
    directionsY.reserve(3);

    directionsY.push_back(0);
    directionsY.push_back(0.2);
    directionsY.push_back(-0.2);
    directionsY.push_back(0.4);
    directionsY.push_back(-0.4);

    //initializing comum values as defaults
    Vec2 interestPosition = Vec2(this->screenSize.width,this->screenSize.height/2);

    for(int i = 0; i<directionsY.size();i++){
        PhysicsRayCastInfo info = applyRayCast(Vec2(1,directionsY.at(i)),dist);
        if(info.shape != nullptr){
            auto node = info.shape->getBody()->getNode();
            if(node->getTag() == GameManager::getInstance()->scoreArea_tag){
                interestPosition = node->getPosition();
                // log("Detectou %s", info.shape->getBody()->getNode()->getName().c_str());
            }   
        }
    }
    
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(this->drawNode,10);
    
}

PhysicsRayCastInfo AgentFlappyBot::applyRayCast(Vec2 direction, float distance){
    auto d = direction*distance;
    Vec2 centerSprite = this->getPosition();
    Vec2 point2 = centerSprite + d;
    PhysicsRayCastInfo infoDetect = PhysicsRayCastInfo();
    infoDetect.shape = nullptr;
    
    Vec2 points[5];

    int num = 0;
    auto func = [&points, &num,&infoDetect](PhysicsWorld& world,
        const PhysicsRayCastInfo& info, void* data)->bool
    {
        if (num < 5)
        {
            if(info.shape != nullptr){  
                points[num++] = info.contact;
                infoDetect = info;
            }
        }
        return true;
    };
    auto scene = Director::getInstance()->getRunningScene();
    scene->getPhysicsWorld()->rayCast(func, centerSprite, point2, nullptr);

    this->drawNode->drawSegment(centerSprite, point2,1,Color4F::RED);

    // filter the closest point based on x
    Vec2 dotPoint = Vec2(10000,0);
    for(int i = 0; i < num ;i++){
        if(points[i].x <= dotPoint.x){
            dotPoint = points[i];
        }
    }

    this->drawNode->drawDot(dotPoint,3,Color4F(1.0f, 1.0f, 1.0f, 1.0f));

    return infoDetect;
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
     
        


