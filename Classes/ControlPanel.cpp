//
//  ControlPanel.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 29/12/19.
//

#include "ControlPanel.hpp"
#include "AcademyFlappyBots.hpp"

USING_NS_CC;

bool ControlPanel::init(){
    Node::init();
    this->initialSetup();
    return true;
}

void ControlPanel::initialSetup(){
    //set the current node size
    this->setContentSize(Size(1024,300));

    //create the white background
    backGroundPanel = DrawNode::create();
    backGroundPanel->drawSolidRect(Vec2::ZERO, Vec2(this->getContentSize().width,300), Color4F::WHITE);
    backGroundPanel->setPosition(Vec2(0,512));
    backGroundPanel->setContentSize(this->getContentSize());
    this->addChild(backGroundPanel);

    auto fakeStrokeSize = 4;
    auto backColor2 = Color4F(0.7686,0.7686,0.7686,1.0);

    //create the training panel
    initialPanel = DrawNode::create();
    auto initialColor = Color4F(0.4627,0.4627,0.4627,1.0);
    initialPanel->drawSolidRect(Vec2::ZERO, Vec2(this->getContentSize().width - 2*fakeStrokeSize,300 - 2*fakeStrokeSize), initialColor);
    initialPanel->setPosition(Vec2(fakeStrokeSize,512 + fakeStrokeSize));
    initialPanel->setContentSize(this->getContentSize());
    this->addChild(initialPanel);


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
    base_nn->setAnchorPoint(Vec2(0,0));
    nnViewerPanel->addChild(base_nn);

    //add parameter base sprite
    base_nn = Sprite::create("Sprites/base_parameters.png");
    base_nn->setScale(0.5);
    base_nn->setAnchorPoint(Vec2(0,0));
    parametersViewerPanel->addChild(base_nn);

    //poputale active neurons vector
    activeNeurons.reserve(7);
    for(int i = 0; i < 7; i++){
        auto neuronSprite = Sprite::create("Sprites/active_neuron.png");
        neuronSprite->setScale(0.5);
        neuronSprite->setAnchorPoint(Vec2(0,1));
        activeNeurons.pushBack(neuronSprite);
        // neuronSprite->setVisible(false);
        nnViewerPanel->addChild(neuronSprite);
    }
    activeNeurons.at(0)->setPosition(Vec2(80,backGroundPanel->getContentSize().height - 98));
    activeNeurons.at(1)->setPosition(Vec2(80,backGroundPanel->getContentSize().height - 178));   
    activeNeurons.at(2)->setPosition(Vec2(236,backGroundPanel->getContentSize().height - 13));  
    activeNeurons.at(3)->setPosition(Vec2(236,backGroundPanel->getContentSize().height - 93)); 
    activeNeurons.at(4)->setPosition(Vec2(236,backGroundPanel->getContentSize().height - 173));
    activeNeurons.at(5)->setPosition(Vec2(236,backGroundPanel->getContentSize().height - 253));
    activeNeurons.at(6)->setPosition(Vec2(399,backGroundPanel->getContentSize().height - 133));

    //poputale active neurons vector
    labelsWeights.reserve(12);
    labelsWeightsNames.reserve(12);
    float startPosX = 250;
    float startPosXName = 200;
    float startPosY = 45;
    float offSetY = 18;
    for(int i = 0; i < 12; i++){
        auto label = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
        auto labelName = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
        label->setTextColor(Color4B::BLACK);
        labelName->setTextColor(Color4B::BLACK);
        label->setAnchorPoint(Vec2(0,1));
        labelName->setAnchorPoint(Vec2(0,1));
        label->setPosition(Vec2(startPosX,backGroundPanel->getContentSize().height - (i*offSetY + startPosY)));
        labelName->setPosition(Vec2(startPosXName,backGroundPanel->getContentSize().height - (i*offSetY + startPosY)));
        labelName->setString("p" + toStr(float(i+1)) + ":");
        labelsWeights.pushBack(label);
        labelsWeightsNames.pushBack(labelName);
        parametersViewerPanel->addChild(label);
        parametersViewerPanel->addChild(labelName);
    }

    //poputale active neurons vector
    labelsOutputs.reserve(7);
    labelsOutputsNames.reserve(7);
    startPosX = 420;
    startPosXName = 370;
    for(int i = 0; i < 7; i++){
        auto label = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
        auto labelName = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
        label->setTextColor(Color4B::BLACK);
        labelName->setTextColor(Color4B::BLACK);
        label->setAnchorPoint(Vec2(0,1));
        labelName->setAnchorPoint(Vec2(0,1));
        
        if (i == 6) {
            label->setPosition(Vec2(startPosX,backGroundPanel->getContentSize().height - (10*offSetY + startPosY)));
            labelName->setPosition(Vec2(startPosXName,backGroundPanel->getContentSize().height - (10*offSetY + startPosY)));
            labelName->setString("out");
        }
        else{
            label->setPosition(Vec2(startPosX,backGroundPanel->getContentSize().height - (i*offSetY + startPosY)));
            labelName->setPosition(Vec2(startPosXName,backGroundPanel->getContentSize().height - (i*offSetY + startPosY)));
            labelName->setString("s" + toStr(float(i+1)) + ":");
        }

        labelsOutputs.pushBack(label);
        labelsOutputsNames.pushBack(labelName);
        parametersViewerPanel->addChild(label);
        parametersViewerPanel->addChild(labelName);
    }

    dxLabel = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
    dxLabel->setTextColor(Color4B::BLACK);
    dxLabel->setAnchorPoint(Vec2(0,1));
    dxLabel->setPosition(Vec2(100,backGroundPanel->getContentSize().height - 45));
    parametersViewerPanel->addChild(dxLabel);

    dxLabelName = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
    dxLabelName->setTextColor(Color4B::BLACK);
    dxLabelName->setAnchorPoint(Vec2(0,1));
    dxLabelName->setPosition(Vec2(50,backGroundPanel->getContentSize().height - 45));
    dxLabelName->setString("dx:");
    parametersViewerPanel->addChild(dxLabelName);

    dyLabel = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
    dyLabel->setTextColor(Color4B::BLACK);
    dyLabel->setAnchorPoint(Vec2(0,1));
    dyLabel->setPosition(Vec2(100,backGroundPanel->getContentSize().height - 65));
    parametersViewerPanel->addChild(dyLabel);

    dyLabelName = Label::createWithTTF("0.00", "fonts/arial.ttf", 14);
    dyLabelName->setTextColor(Color4B::BLACK);
    dyLabelName->setAnchorPoint(Vec2(0,1));
    dyLabelName->setString("dy:");
    dyLabelName->setPosition(Vec2(50,backGroundPanel->getContentSize().height - 65));
    parametersViewerPanel->addChild(dyLabelName);


    graphTrainingMode = DrawNode::create();
    graphTrainingMode->setContentSize(Size(initialPanel->getContentSize().width,initialPanel->getContentSize().height));
    graphTrainingMode->drawSolidRect(Vec2::ZERO, Vec2(graphTrainingMode->getContentSize().width,graphTrainingMode->getContentSize().height),Color4F::BLACK);
    graphTrainingMode->setPosition(Vec2::ZERO);
    initialPanel->addChild(graphTrainingMode);

    this->setNoMode();
    this->schedule();
}

void ControlPanel::schedule(){
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(ControlPanel::update, this), this, 1.0f / 60, false, "controlPanel");
}

void ControlPanel::unschedule(){
    Director::getInstance()->getScheduler()->unschedule("controlPanel", this);
}

void ControlPanel::update(float dt){  

    if(!isActive) return;

    if(AcademyFlappyBots::getInstance()->inferenceModeOn){
        for(int i = 0 ; i < 7; i++){
            float output = AcademyFlappyBots::getInstance()->inferenceBird->nn->currentOutputs.at(i);
            activeNeurons.at(i)->setOpacity(output*400);
            if(i == 0 || i == 1 || i == 6){
                activeNeurons.at(i)->setVisible(output > 0.5);
            }
            
            labelsOutputs.at(i)->setString(this->toStr(output));
        }
        auto weights = AcademyFlappyBots::getInstance()->inferenceBird->nn->getWeightsAsVector();
        for(int i = 0 ; i < 12; i++){
            labelsWeights.at(i)->setString(this->toStr(weights.at(i)));
        }
        dxLabel->setString(this->toStr(AcademyFlappyBots::getInstance()->inferenceBird->dx));
        dyLabel->setString(this->toStr(AcademyFlappyBots::getInstance()->inferenceBird->dy));
    }
    else{
        auto orig = lastPoint;
        auto dest = Vec2(AcademyFlappyBots::getInstance()->getCurrentGeneration(),AcademyFlappyBots::getInstance()->cumulatedReward);
        graphTrainingMode->drawLine(orig, dest, Color4F::GREEN);
        lastPoint = dest;
    }
}

void ControlPanel::startTrainingPressed(){
    log("startTrainingPressed pressed");
}
void ControlPanel::watchBestPlayerPressed(){
    log("watchBestPlayerPressed");
}

void ControlPanel::setTrainingMode(){
    nnViewerPanel->setVisible(false);
    parametersViewerPanel->setVisible(false);
    initialPanel->setVisible(true);
    // graphTrainingMode->setVisible(true);
    isActive = true;
}

void ControlPanel::setInferenceMode(){
    initialPanel->setVisible(false);
    nnViewerPanel->setVisible(true);
    parametersViewerPanel->setVisible(true);
    // graphTrainingMode->setVisible(false);
    isActive = true;
}

void ControlPanel::setNoMode(){
    initialPanel->setVisible(false);
    nnViewerPanel->setVisible(false);
    parametersViewerPanel->setVisible(false);
    // graphTrainingMode->setVisible(false);
    isActive = false;
}

// template <typename T> string tostr(const T& t){ 
//    ostringstream os; 
//    os<<t; 
//    return os.str(); 
// } 

string ControlPanel::toStr(float val){
    stringstream ss2 (stringstream::in | stringstream::out);
    ss2.precision(3);
    ss2 << val;
    string dyStr = ss2.str();
    return dyStr;
}

