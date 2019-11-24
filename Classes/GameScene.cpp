//
//  GameScene.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "GameScene.hpp"
#include "Column.hpp"
#include "Respawner.hpp"
#include "ScrollableSprite.hpp"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    setupScreen(origin, visibleSize);
    return true;
}

void GameScene::setupScreen(Vec2 origin, Size visibleSize){
   
    auto spritecache = loadSpriteSheet();

    auto sky = ScrollableSprite::create(true);
    auto ground = ScrollableSprite::create(true);
    auto trees = ScrollableSprite::create(true);

    sky->initWithSpriteFrameName("sky_repeated.png");
    ground->initWithSpriteFrameName("ground.png");
    trees->initWithSpriteFrameName("trees.png");

    sky->setAnchorPoint(Vec2(0,0));
    ground->setAnchorPoint(Vec2(0,0.5));
    trees->setAnchorPoint(Vec2(0,0));

    auto skyRate = 1.0;
    auto treesRate = 1.3;
    auto groundRate = 1.8;

    sky->setRate(skyRate);
    ground->setRate(groundRate);
    trees->setRate(treesRate);

    auto parallaxInitPos = Vec2(0,0);
    auto parallaxEndPos = Vec2(-visibleSize.width,0);

    sky->setInitPosition(parallaxInitPos);
    sky->setEndPosition(parallaxEndPos);
    ground->setInitPosition(parallaxInitPos);
    ground->setEndPosition(parallaxEndPos);
    trees->setInitPosition(parallaxInitPos);
    trees->setEndPosition(parallaxEndPos);

    sky->start();
    ground->start();
    trees->start();

    this->addChild(sky,-3);
    this->addChild(trees,-2);
    this->addChild(ground,-1);  
    
    auto player = Sprite::createWithSpriteFrameName("BirdHero0.png");
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
    animFrames.pushBack(spritecache->spriteFrameByName("BirdHero0.png"));
    animFrames.pushBack(spritecache->spriteFrameByName("BirdHero1.png"));
   
    Animation* anim = Animation::createWithSpriteFrames(animFrames,0.15f);
    Animate* animate = Animate::create(anim);

    player->runAction(RepeatForever::create(animate));

    player->setPosition(Vec2(300,visibleSize.height/2));
    player->setScale(0.4);
    this->addChild(player);

    auto moveBy = MoveBy::create(1,Vec2(0,100));
    auto delay = DelayTime::create(0.1);
    auto seq = Sequence::create(moveBy,delay,moveBy->reverse(),delay->clone(),nullptr);
    player->runAction(RepeatForever::create(seq));

    auto respawner = Respawner::create();
    respawner->setSceneNode(this);
    respawner->setSpeed(1.8);
    respawner->start();
    this->addChild(respawner);
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
    return true;
}

SpriteFrameCache* GameScene::loadSpriteSheet(){
    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("Sprites/SpriteSheet.plist");
    return spritecache;
}


