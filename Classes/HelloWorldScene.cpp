/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // addExitButton(origin, visibleSize);

    setupScreen(origin, visibleSize);
   
    return true;
}

void HelloWorld::addExitButton(Vec2 origin, Size visibleSize){

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

void HelloWorld::setupScreen(Vec2 origin, Size visibleSize)
{

    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("Sprites/SpriteSheet.plist");

    auto sky = Sprite::createWithSpriteFrameName("sky_repeated.png");
    auto ground = Sprite::createWithSpriteFrameName("ground.png");
    auto trees = Sprite::createWithSpriteFrameName("trees.png");
    sky->setAnchorPoint(Vec2(0,0));
    ground->setAnchorPoint(Vec2(0,0.5));
    trees->setAnchorPoint(Vec2(0,0));

    auto skyRate = 1.0;
    auto treesRate = 1.3;
    auto groundRate = 1.8;

    this->addChild(sky,-3);
    this->addChild(trees,-2);
    this->addChild(ground,-1);  
 
    this->scrollSprite(sky,skyRate,visibleSize);
    this->scrollSprite(trees,treesRate,visibleSize);
    this->scrollSprite(ground,groundRate,visibleSize);

    
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

    auto startLabel = Label::createWithTTF("Tap to start", "fonts/Marker Felt.ttf", 64);
    startLabel->enableShadow();
    auto callbackStart = [&](Ref* sender){
         log("Clicando no item do menu.\n");
    };
    auto startMenuItem  = MenuItemLabel::create(startLabel,callbackStart);
    auto myMenu = Menu::createWithItem(startMenuItem); 
    this->addChild(myMenu,10);

    auto button = Button::create("Sprites/bt_normal.png","Sprites/bt_pressed.png");
    button->setPosition(Vec2(visibleSize.width/2,visibleSize.height*0.3));
    button->setAnchorPoint(Vec2(0.5f,0.5f));
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            log("Button pressed.\n");
            break;
        case Widget::TouchEventType::ENDED:
             log("Button released.\n");
        default:
            break;
        }
    });
    
    this->addChild(button);
    //  auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    // if (label == nullptr)
    // {
    //     problemLoading("'fonts/Marker Felt.ttf'");
    // }
    // else
    // {
    //     // position the label on the center of the screen
    //     label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                             origin.y + visibleSize.height - label->getContentSize().height));

    //     // add the label as a child to this layer
    //     this->addChild(label, 1);
    // }

    // // add "HelloWorld" splash screen"
    // auto sprite = Sprite::create("HelloWorld.png");
    // if (sprite == nullptr)
    // {
    //     problemLoading("'HelloWorld.png'");
    // }
    // else
    // {
    //     // position the sprite on the center of the screen
    //     sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //     // add the sprite as a child to this layer
    //     this->addChild(sprite, 0);
    // }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    log("estou clicando no label");
    //Close the cocos2d-x game scene and quit the application
    // Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::scrollSprite(Sprite *sprite, float rate, Size visibleSize,float baseSpeed){
    auto move = MoveTo::create(baseSpeed/rate,Vec2(-visibleSize.width,0));
    auto resetPos = MoveTo::create(0,Vec2::ZERO);
    auto sequence = Sequence::create(move,resetPos,nullptr);
    sprite->runAction(RepeatForever::create(sequence));
}
