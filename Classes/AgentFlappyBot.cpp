//
//  AgentFlappyBot.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#include "AgentFlappyBot.hpp"
#include "GameManager.hpp"
#include "Column.hpp"

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

    auto scoreAreaPos = this->observe();
    auto currentPos = this->getPosition();
    vector<float> resp;
    resp.reserve(2);
    //simulating distance x
    float val1 = (scoreAreaPos.x - currentPos.x)/this->screenSize.width;
    //simulation distance y
    float val2 = (scoreAreaPos.y - currentPos.y)/this->screenSize.height;
    //getting y position
    // float val3 = this->getPosition().y/this->screenSize.height;

    resp.push_back(val1);
    resp.push_back(val2);
    // resp.push_back(val3);

    // log("Obs: %3,2f\t%3,2f\t%3,2f",val1,val2,val3);

    return resp;
}

Vec2 AgentFlappyBot::observe(){

    this->clearDrawnNode();
    this->drawNode = DrawNode::create();
    float dist = this->screenSize.width - this->getPosition().x*1.1f;

    vector<float> directionsY;
    directionsY.reserve(1);

    directionsY.push_back(0);
    //initializing comum values as defaults
    Vec2 interestPosition = Vec2(this->screenSize.width,this->screenSize.height/2);

    for(int i = 0; i<directionsY.size();i++){
        PhysicsRayCastInfo info = applyRayCast(Vec2(1,directionsY.at(i)),dist, false);
        if(info.shape != nullptr){
            auto node = info.shape->getBody()->getNode();
            if(node->getTag() == GameManager::getInstance()->scoreArea_tag || 
            node->getTag() == GameManager::getInstance()->column_tag){
                interestPosition = ((Column*)(node->getParent()))->getPosition();
            }   
        }
    }
    
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(this->drawNode,10);

    return interestPosition;
}

PhysicsRayCastInfo AgentFlappyBot::applyRayCast(Vec2 direction, float distance, bool shouldDrawn){
    auto d = direction*distance;
    Vec2 centerSprite = this->getPosition();
    Vec2 point2 = centerSprite + d;
    PhysicsRayCastInfo infoDetect = PhysicsRayCastInfo();
    infoDetect.shape = nullptr;
    
    Vec2 points[1];
    int num = 0;
    auto func = [&points, &num,&infoDetect](PhysicsWorld& world,
        const PhysicsRayCastInfo& info, void* data)->bool
    {
        if(num < 1){
            if(info.shape != nullptr){  
                if(info.shape->getBody()->getNode()->getTag() == GameManager::getInstance()->scoreArea_tag || 
                info.shape->getBody()->getNode()->getTag() == GameManager::getInstance()->column_tag){
                    
                    infoDetect = info;
                    points[num++] = info.contact;
                    return false;
                }  
            }
        } 
        return true;
    };
    auto scene = Director::getInstance()->getRunningScene();
    scene->getPhysicsWorld()->rayCast(func, centerSprite, point2, nullptr);
    
    if(!shouldDrawn){
         return infoDetect;
    }

    this->drawNode->drawSegment(centerSprite, point2,0.5,Color4F::RED);
    // filter the closest point based on x
    // Vec2 dotPoint = Vec2(10000,0);
    // for(int i = 0; i < num ;i++){
    //     if(points[i].x <= dotPoint.x){
    //         dotPoint = points[i];
    //     }
    // }
    this->drawNode->drawDot(points[0],3,Color4F(1.0f, 1.0f, 1.0f, 1.0f));

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
     
void AgentFlappyBot::die(){
    Player::die(); 
    this->clearDrawnNode();
}

void AgentFlappyBot::clearDrawnNode(){
    if(this->drawNode){
        this->drawNode->removeFromParentAndCleanup(true);
        this->drawNode = nullptr;
    }
}


void AgentFlappyBot::setTotalScore(int total){
    this->totalScore = total;
}
        


