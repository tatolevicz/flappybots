//
//  Splash.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "Splash.hpp"
#include "GameScene.hpp"
#include "TrainingScene.hpp"

USING_NS_CC;

Scene* Splash::createScene()
{
    return Splash::create();
}

bool Splash::init()
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

void Splash::setupScreen(Vec2 origin, Size visibleSize){
    auto splashImage = Sprite::create("Sprites/BirdHero0.png");
    splashImage->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    splashImage->setOpacity(0);
    this->addChild(splashImage);  

    auto fadeIn = FadeIn::create(0.5f);
    auto delay = DelayTime::create(1.0f);
    auto callbackGoGame = CallFunc::create([](){
        Director::getInstance()->replaceScene(TrainingScene::createScene());
    });
    auto seq = Sequence::create(fadeIn,delay,fadeIn->reverse(),callbackGoGame,nullptr);
    splashImage->runAction(seq);
}



