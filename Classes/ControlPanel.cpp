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
    initialPanel->drawSolidRect(Vec2::ZERO, Vec2(this->getContentSize().width,300), Color4F::WHITE);
    initialPanel->setPosition(Vec2(0,512));
    initialPanel->setContentSize(this->getContentSize());
    this->addChild(initialPanel);

    //create nn viewer panel on the right and hide it
    // nnViewerPanel = DrawNode::create();
    // nnViewerPanel->drawSolidRect(Vec2(this->getContentSize().width/2,0), Vec2(this->getContentSize().width/2,this->getContentSize().height), Color4F::WHITE);
    // nnViewerPanel->setContentSize(this->getContentSize());
    // this->addChild(nnViewerPanel);

    //create parameters viewer panel on the left and hide it
}


void ControlPanel::startTrainingPressed(){
    log("startTrainingPressed pressed");
}
void ControlPanel::watchBestPlayerPressed(){
    log("watchBestPlayerPressed");
}

