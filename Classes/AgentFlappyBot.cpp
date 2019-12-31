//
//  AgentFlappyBot.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#include "AgentFlappyBot.hpp"
#include "GameManager.hpp"
#include "Column.hpp"
#include "AcademyFlappyBots.hpp"

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
    this->initIA();
    this->weights = this->nn->getWeightsAsVector();
}

vector<float> AgentFlappyBot::collectObservations(){

    auto scoreAreaPos = this->observe();
    auto currentPos = this->getPosition();
    vector<float> resp;
    resp.reserve(2);
    float val1 = (scoreAreaPos.x - currentPos.x)/this->screenSize.width;
    float val2 = (scoreAreaPos.y - currentPos.y)/this->screenSize.height;
    this->dx = val1;
    this->dy = val2;
    resp.push_back(val1);
    resp.push_back(val2);
    return resp;
}

Vec2 AgentFlappyBot::observe(){
    this->clearDrawnNode();
    this->drawNode = DrawNode::create();
    float dist = this->screenSize.width - this->screenSize.width*0.75;
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
                interestPosition = ((Column*)(node->getParent()))->getScoreAreaPosition();
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

     if(infoDetect.shape){
        this->drawNode->drawDot(((Column*)infoDetect.shape->getBody()->getNode()->getParent())->getScoreAreaPosition(),3,Color4F::YELLOW);
    }

    this->drawNode->drawSegment(centerSprite, point2,0.5,Color4F::RED);
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
    this->nn->setWeightsFromVector(newWeights);
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

void AgentFlappyBot::initIA(){
    this->nn = NeuralNetwork::create();
    this->nn->retain();

    int numberOfHiddenLayers = AcademyFlappyBots::getInstance()->numberOfHiddenLayers;
    int hiddenLayerSize[1] = {AcademyFlappyBots::getInstance()->hiddenLayersSize};
    int numberOfInputs = AcademyFlappyBots::getInstance()->numberOfInputs;
    int numberOfOutputs = AcademyFlappyBots::getInstance()->numberOfOutputs; 
    if(nn->init(numberOfInputs,numberOfHiddenLayers,hiddenLayerSize,numberOfOutputs)){
        // log("NeuralNetwork initialized");
    }
}

int AgentFlappyBot::getNumberOfWeights(){
    return this->nn->getNumberOfConnections();
}


