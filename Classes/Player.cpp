//
//  Player.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Player.hpp"
#include "GameManager.hpp"

Player::Player(){}
Player::~Player(){
    unschedule();
}

Player* Player::create(){
    Player* player = new Player();

    if(player && player->init()){
        player->autorelease();     
        return player;
    }

    CC_SAFE_DELETE(player);
    return NULL;    
}

bool Player::init()
{
    this->initialSetup();
    return true;
}

void Player::initialSetup(){
    this->screenSize = Director::getInstance()->getVisibleSize();
    this->addAnimation();
    this->addPhysics();
    this->schedule();
}

void Player::addAnimation(){
    auto spriteSheet = SpriteFrameCache::getInstance();
    this->initWithSpriteFrameName("BirdHero0.png");
    this->animFrames.reserve(2);
    this->animFrames.pushBack(spriteSheet->spriteFrameByName("BirdHero0.png"));
    this->animFrames.pushBack(spriteSheet->spriteFrameByName("BirdHero1.png"));
   
    Animation* anim = Animation::createWithSpriteFrames(this->animFrames,0.15f);
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
    // physicsBodyPlayer->setRotationEnable(false);
    physicsBodyPlayer->setCategoryBitmask(GameManager::getInstance()->player_bit_mask_category);
    physicsBodyPlayer->setCollisionBitmask(  GameManager::getInstance()->ground_bit_mask_category | 
                                                GameManager::getInstance()->obstacle_bit_mask_category);
    physicsBodyPlayer->setContactTestBitmask(   GameManager::getInstance()->ground_bit_mask_category | 
                                                GameManager::getInstance()->obstacle_bit_mask_category);

    this->setName(GameManager::getInstance()->player_name);
    this->setTag(GameManager::getInstance()->player_tag);
    this->addComponent(physicsBodyPlayer);
}

void Player::jump(){
    if(this->isDead)return;
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    this->getPhysicsBody()->setAngularVelocity(0.0f);
    auto direction = Vec2(-sinf(this->currentAngle*3.1416/180.0f),cosf(this->currentAngle*3.1416/180.0f));
    this->getPhysicsBody()->applyImpulse(direction*GameManager::getInstance()->jumpForce);
    this->flap();
}

void Player::stopAnimation(){
    this->stopAllActions();
}

void Player::flap(){
    Animation* anim = Animation::createWithSpriteFrames(this->animFrames,0.15f);
    Animate* animate = Animate::create(anim);
    this->runAction(animate);
}

void Player::die(){
    this->isDead = true;
    auto spriteSheet = SpriteFrameCache::getInstance();
    this->setSpriteFrame(spriteSheet->getSpriteFrameByName("BirdHero2.png"));
    this->currentAngle = 0;
    this->getPhysicsBody()->setEnabled(false);
}

void Player::reset(){
    this->isDead = false;
    this->resetScore();
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    this->getPhysicsBody()->setAngularVelocity(0.0f);
    this->setRotation(0);
    auto spriteSheet = SpriteFrameCache::getInstance();
    this->setSpriteFrame(spriteSheet->getSpriteFrameByName("BirdHero1.png"));
    this->setPosition(Vec2(300,this->screenSize.height/2));
    this->getPhysicsBody()->setEnabled(true);
}

void Player::schedule(){
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(Player::update, this), this, 1.0f / 60, false, "player");
}

void Player::unschedule(){
    Director::getInstance()->getScheduler()->unschedule("player", this);
}

void Player::update(float dt){
    if(this->isDead)return;
    if(GameManager::getInstance()->state == GameManager::PLAYING_STATE){
        this->rotate(dt);
    }
}

void Player::rotate(float dt){
    auto currentPosition = this->getPosition().y;
    if(lastYPosition > currentPosition){
        //going down
        this->currentAngle = clampf(this->currentAngle += dt+this->rotationRate/2,-this->maxAngle,this->maxAngle);
    }
    else if(lastYPosition < currentPosition){
        //going up
        this->currentAngle = clampf(this->currentAngle -= dt+this->rotationRate,-this->maxAngle,this->maxAngle);
    }
    this->setRotation(this->currentAngle);
    this->lastYPosition = currentPosition;

}

int Player::getTotalScore(){
    return this->totalScore;
}

void Player::resetScore(){
    this->totalScore = 0;
}

void Player::score(){
    this->totalScore += 1;
    log("Agent score: %d", this->totalScore);
}

bool Player::getIsDead(){
    return this->isDead;
}
