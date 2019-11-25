//
//  GameScene.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "GameScene.hpp"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::initWithPhysics() )
    {
        return false;
    }
    this->screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    setupScreen(origin);
    return true;
}

void GameScene::setupScreen(Vec2 origin){
    loadSpriteSheet();
    createParallax();
    addPlayer();
    createRespawner();
    createUI();
    addPhysicsGround();
    setPhysicsParameters();
    setupInput();
}

void GameScene::loadSpriteSheet(){
    // load the Sprite Sheet
    this->spriteSheet = SpriteFrameCache::getInstance();
    // the .plist file can be generated with any of the tools mentioned below
    spriteSheet->addSpriteFramesWithFile("Sprites/SpriteSheet.plist");
}

void GameScene::createParallax(){
    this->sky = ScrollableSprite::create(true);
    this->ground = ScrollableSprite::create(true);
    this->trees = ScrollableSprite::create(true);

    this->sky->initWithSpriteFrameName("sky_repeated.png");
    this->ground->initWithSpriteFrameName("ground.png");
    this->trees->initWithSpriteFrameName("trees.png");

    this->sky->setAnchorPoint(Vec2(0,0));
    this->ground->setAnchorPoint(Vec2(0,0.5));
    this->trees->setAnchorPoint(Vec2(0,0));

    auto skyRate = 1.0;
    auto treesRate = 1.3;
    auto groundRate = 1.8;

    this->sky->setRate(skyRate);
    this->ground->setRate(groundRate);
    this->trees->setRate(treesRate);

    auto parallaxInitPos = Vec2(0,0);
    auto parallaxEndPos = Vec2(-this->screenSize.width,0);

    this->sky->setInitPosition(parallaxInitPos);
    this->sky->setEndPosition(parallaxEndPos);
    this->ground->setInitPosition(parallaxInitPos);
    this->ground->setEndPosition(parallaxEndPos);
    this->trees->setInitPosition(parallaxInitPos);
    this->trees->setEndPosition(parallaxEndPos);

    this->sky->start();
    this->ground->start();
    this->trees->start();

    this->addChild(this->sky,-3);
    this->addChild(this->trees,-2);
    this->addChild(this->ground,-1);  
}

void GameScene::addPlayer(){
    this->player = Player::create();
    this->addChild(player,1);
}

void  GameScene::createRespawner(){
    this->respawner = Respawner::create();
    this->respawner->setSceneNode(this);
    this->respawner->setSpeed(1.8);
    this->addChild(this->respawner);
}

void GameScene::createUI(){
    auto startLabel = Label::createWithTTF("Tap to start", "fonts/Marker Felt.ttf", 64);
    startLabel->enableShadow();
    auto callbackStart = [&](Ref* sender){
         this->startButtonPressed(this->startButton);
    };
    auto startButtonItem  = MenuItemLabel::create(startLabel,callbackStart);
    this->startButton = Menu::createWithItem(startButtonItem); 
    this->addChild(this->startButton,10);
}

void GameScene::startButtonPressed(Ref* pSender){
     this->respawner->start();
     this->startButton->setVisible(false);
     this->player->stopAnimation();
     this->player->getPhysicsBody()->setGravityEnable(true);
     GameManager::getInstance()->state = GameManager::PLAYING_STATE;
}
 
void  GameScene::addPhysicsGround(){
    auto sizeBodyGround = Size(this->screenSize.width,this->screenSize.height*0.10);
    auto physicsBodyGround = PhysicsBody::createBox(sizeBodyGround,PHYSICSBODY_MATERIAL_DEFAULT);
    physicsBodyGround->setDynamic(false);

    this->groundCollider = Sprite::create();
    groundCollider->setAnchorPoint(Vec2::ZERO);
    groundCollider->setContentSize(sizeBodyGround);

    groundCollider->setName(GameManager::getInstance()->ground_name);
    groundCollider->setTag(GameManager::getInstance()->ground_tag);

    physicsBodyGround->setCategoryBitmask(GameManager::getInstance()->ground_bit_mask_category);
    physicsBodyGround->setContactTestBitmask(GameManager::getInstance()->player_bit_mask_category);

    groundCollider->addComponent(physicsBodyGround);

    this->addChild(this->groundCollider);
}

void GameScene::setPhysicsParameters(){
    this->getPhysicsWorld()->setDebugDrawMask(  GameManager::getInstance()->ground_bit_mask_category | 
                                                GameManager::getInstance()->player_bit_mask_category |
                                                GameManager::getInstance()->obstacle_bit_mask_category);
    this->getPhysicsWorld()->setSpeed(GameManager::getInstance()->gravitySpeed);

    auto contactListenerIn = EventListenerPhysicsContact::create();
    contactListenerIn->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenerIn, this);

    auto contactListenerOut = EventListenerPhysicsContact::create();
    contactListenerOut->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenerOut, this);
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
    if(this->getState() != GameManager::PLAYING_STATE){return false;}
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if(!isPlayerContact(nodeA,nodeB)){return false;}

    if (nodeA && nodeB)
    {
        auto result = this->checkCollision(nodeA, nodeB);
        if(result){
            this->gameOver();
        }
        return result;
    }
    return false;
}

void GameScene::onContactSeparate(PhysicsContact& contact){

    if(this->getState() != GameManager::PLAYING_STATE){return;}

    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if(!isPlayerContact(nodeA,nodeB)){return;}
    if (nodeA && nodeB)
    {
        auto result = this->checkCollision(nodeA, nodeB);
        if(!result){
            log("Player Scored.");
        }
    }
}

bool GameScene::checkCollision(Node* nodeA, Node* nodeB){
          
    if (nodeA->getTag() == GameManager::getInstance()->column_tag || nodeB->getTag() == GameManager::getInstance()->column_tag)
    { 
        return true;
    }

    if (nodeA->getTag() == GameManager::getInstance()->ground_tag || nodeB->getTag() == GameManager::getInstance()->ground_tag)
    {
        return true;
    }

    if (nodeA->getTag() == GameManager::getInstance()->scoreArea_tag || nodeB->getTag() == GameManager::getInstance()->scoreArea_tag)
    {
        return false;
    }
    
    return false;
}

bool GameScene::isPlayerContact(Node* nodeA, Node* nodeB){
    auto isPlayer = nodeA->getTag() == GameManager::getInstance()->player_tag || 
                nodeB->getTag() == GameManager::getInstance()->player_tag;
    return isPlayer;
}

void GameScene::setupInput(){
    auto touchListener = EventListenerTouchOneByOne::create();
   touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
   _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    if(this->getState() == GameManager::FINISHED_STATE) {
        this->restartGame();
        return true;
    }
//    log("You tap the screen");
    this->player->jump();
    return true;
}

int GameScene::getState(){
    // log("GetState : %d", GameManager::getInstance()->state);
    return GameManager::getInstance()->state;
}

void GameScene::gameOver(){
    log("Player Died.");
    this->player->die();
    this->stopScene();
}

void GameScene::stopScene(){
    this->respawner->stop();
    for(int i = 0; i < this->getChildrenCount(); i++){
        this->getChildren().at(i)->stopAllActions();
    }
    GameManager::getInstance()->state = GameManager::FINISHED_STATE;
}

void GameScene::restartGame(){
    this->player->reset();
    this->respawner->restart();
    this->sky->setPosition(Vec2::ZERO);
    this->ground->setPosition(Vec2::ZERO);
    this->trees->setPosition(Vec2::ZERO);
    this->sky->start();
    this->ground->start();
    this->trees->start();
    GameManager::getInstance()->state = GameManager::PLAYING_STATE;
}