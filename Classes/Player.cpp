//
//  Player.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Player.hpp"
#include "GameManager.hpp"

Player::Player(){}
Player::~Player(){}

Player* Player::create(){

    Player* player = new Player();

    if(player){
        player->autorelease();
        player->initialSetup();
        return player;
    }

    CC_SAFE_DELETE(player);
    return NULL;    
}

void Player::initialSetup(){
    this->screenSize = Director::getInstance()->getVisibleSize();
    this->addAnimation();
    this->addPhysics();
}

void Player::addAnimation(){
    auto spriteSheet = SpriteFrameCache::getInstance();
    this->initWithSpriteFrameName("BirdHero0.png");
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
    animFrames.pushBack(spriteSheet->spriteFrameByName("BirdHero0.png"));
    animFrames.pushBack(spriteSheet->spriteFrameByName("BirdHero1.png"));
   
    Animation* anim = Animation::createWithSpriteFrames(animFrames,0.15f);
    Animate* animate = Animate::create(anim);

    this->runAction(RepeatForever::create(animate));

    this->setPosition(Vec2(300,this->screenSize.height/2));
    this->setScale(0.4);

    auto moveBy = MoveBy::create(1,Vec2(0,100));
    auto delay = DelayTime::create(0.1);
    auto seq = Sequence::create(moveBy,delay,moveBy->reverse(),delay->clone(),nullptr);
    this->runAction(RepeatForever::create(seq));
}

void Player::addPhysics(){
    auto physicsBodyPlayer = PhysicsBody::createCircle(this->getContentSize().width*0.33,PHYSICSBODY_MATERIAL_DEFAULT);
    physicsBodyPlayer->setGravityEnable(false);
    physicsBodyPlayer->setCategoryBitmask(GameManager::getInstance()->player_bit_mask_category);
    physicsBodyPlayer->setCollisionBitmask(GameManager::getInstance()->ground_bit_mask_category | GameManager::getInstance()->obstacle_bit_mask_category);
    physicsBodyPlayer->setContactTestBitmask(GameManager::getInstance()->ground_bit_mask_category);
    this->setName(GameManager::getInstance()->player_name);
    this->setTag(GameManager::getInstance()->player_tag);
    this->addComponent(physicsBodyPlayer);
}

void Player::jump(){
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    this->getPhysicsBody()->setAngularVelocity(0.0f);
    this->getPhysicsBody()->applyImpulse(Vec2(0,1)*GameManager::getInstance()->jumpForce);
}

void Player::stopAnimation(){
    this->stopAllActions();
}
