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
     this->player->jump();
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
    physicsBodyGround->setCollisionBitmask(GameManager::getInstance()->player_bit_mask_category);
    physicsBodyGround->setContactTestBitmask(GameManager::getInstance()->player_bit_mask_category);

    groundCollider->addComponent(physicsBodyGround);

    this->addChild(this->groundCollider);
}

void GameScene::setPhysicsParameters(){
    this->getPhysicsWorld()->setDebugDrawMask(GameManager::getInstance()->ground_bit_mask_category | GameManager::getInstance()->player_bit_mask_category);
    this->getPhysicsWorld()->setSpeed(GameManager::getInstance()->gravitySpeed);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    log("Contact: node A name: %s. node B name: %s",nodeA->getName().c_str(),nodeB->getName().c_str());
    if (nodeA && nodeB)
    {
        // log("Contact 2: tag: %d", nodeB->getTag());
        // if (nodeA->getTag() == 1)
        // {
        //     nodeB->removeFromParentAndCleanup(true);
        // }
        // else if (nodeB->getTag() == 1)
        // {
        //     nodeA->removeFromParentAndCleanup(true);
        // }
    }

    //bodies can collide
    return false;
}

void GameScene::setupInput(){
    auto touchListener = EventListenerTouchOneByOne::create();
   touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
   _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
   log("You tap the screen");
   this->player->jump();
   return true;
}