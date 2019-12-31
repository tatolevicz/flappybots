//
//  ControlPanel.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 29/12/19.
//

#include "ControlPanel.hpp"

USING_NS_CC;

bool ControlPanel::init(){
    Node::init();
    this->initialSetup();
    return true;
}

void ControlPanel::initialSetup(){
    //set the current node size
    this->setContentSize(Size(1024,300));

    //create the initial options: start training | watch best player
    initialPanel = DrawNode::create();
    auto backColor = Color4F(0.4627,0.4627,0.4627,1.0);
    initialPanel->drawSolidRect(Vec2::ZERO, Vec2(this->getContentSize().width,300), Color4F::WHITE);
    initialPanel->setPosition(Vec2(0,512));
    initialPanel->setContentSize(this->getContentSize());
    this->addChild(initialPanel);


    auto fakeStrokeSize = 4;
    auto backColor2 = Color4F(0.7686,0.7686,0.7686,1.0);

    //create nn viewer panel on the left and hide it
    nnViewerPanel = DrawNode::create();
    nnViewerPanel->setContentSize(Size(this->getContentSize().width/2 - fakeStrokeSize*2,300 - fakeStrokeSize*2));
    nnViewerPanel->drawSolidRect(Vec2::ZERO, Vec2(nnViewerPanel->getContentSize().width,nnViewerPanel->getContentSize().height),backColor2);
    nnViewerPanel->setPosition(Vec2(fakeStrokeSize,512 + fakeStrokeSize));
    this->addChild(nnViewerPanel);

    //create parameters viewer panel on the right and hide it
    parametersViewerPanel = DrawNode::create();
    parametersViewerPanel->setContentSize(Size(this->getContentSize().width/2 - fakeStrokeSize*2,300 - fakeStrokeSize*2));
    parametersViewerPanel->drawSolidRect(Vec2::ZERO, Vec2(parametersViewerPanel->getContentSize().width,parametersViewerPanel->getContentSize().height),backColor2);
    parametersViewerPanel->setPosition(Vec2(this->getContentSize().width/2 + fakeStrokeSize,512 + fakeStrokeSize));
    this->addChild(parametersViewerPanel);

    //add nn base sprite
    base_nn = Sprite::create("Sprites/nn_base.png");
    base_nn->setScale(0.5);
    base_nn->setPosition(0,512);
    base_nn->setAnchorPoint(Vec2(0,0));
    this->addChild(base_nn);
    
}


void ControlPanel::startTrainingPressed(){
    log("startTrainingPressed pressed");
}
void ControlPanel::watchBestPlayerPressed(){
    log("watchBestPlayerPressed");
}

