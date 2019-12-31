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

    //poputale active neurons vector
    activeNeurons.reserve(7);
    for(int i = 0; i < 7; i++){
        auto neuronSprite = Sprite::create("Sprites/active_neuron.png");
        neuronSprite->setScale(0.5);
        neuronSprite->setAnchorPoint(Vec2(0,1));
        activeNeurons.pushBack(neuronSprite);
        neuronSprite->setVisible(false);
        this->addChild(neuronSprite);
    }
    activeNeurons.at(0)->setPosition(Vec2(80,812 - 98));
    activeNeurons.at(1)->setPosition(Vec2(80,812 - 178));   
    activeNeurons.at(2)->setPosition(Vec2(236,812 - 13));  
    activeNeurons.at(3)->setPosition(Vec2(236,812 - 93)); 
    activeNeurons.at(4)->setPosition(Vec2(236,812 - 173));
    activeNeurons.at(5)->setPosition(Vec2(236,812 - 253));
    activeNeurons.at(6)->setPosition(Vec2(399,812 - 133));

    //poputale active neurons vector
    labelsWeights.reserve(12);
    for(int i = 0; i < 12; i++){
        auto label = Label::createWithTTF("0.00", "fonts/arial.ttf", 12);
        label->setTextColor(Color4B::BLACK);
        label->setAnchorPoint(Vec2(0,1));
        labelsWeights.pushBack(label);
        this->addChild(label);
    }
    labelsWeights.at(0)->setPosition(Vec2(721,812 - 61));
    labelsWeights.at(1)->setPosition(Vec2(721,812 - 84));
    labelsWeights.at(2)->setPosition(Vec2(721,812 - 106));
    labelsWeights.at(3)->setPosition(Vec2(721,812 - 128));
    labelsWeights.at(4)->setPosition(Vec2(721,812 - 150));
    labelsWeights.at(5)->setPosition(Vec2(721,812 - 173));
    labelsWeights.at(6)->setPosition(Vec2(721,812 - 195));
    labelsWeights.at(7)->setPosition(Vec2(721,812 - 217));
    labelsWeights.at(8)->setPosition(Vec2(810,812 - 106));
    labelsWeights.at(9)->setPosition(Vec2(810,812 - 128));
    labelsWeights.at(10)->setPosition(Vec2(810,812 - 150));
    labelsWeights.at(11)->setPosition(Vec2(810,812 - 173));


    //poputale active neurons vector
    labelsOutputs.reserve(7);
    for(int i = 0; i < 7; i++){
        auto label = Label::createWithTTF("0.00", "fonts/arial.ttf", 12);
        label->setTextColor(Color4B::BLACK);
        label->setAnchorPoint(Vec2(0,1));
        labelsOutputs.pushBack(label);
        this->addChild(label);
    }
    labelsOutputs.at(0)->setPosition(Vec2(938,812 - 61));
    labelsOutputs.at(1)->setPosition(Vec2(938,812 - 84));
    labelsOutputs.at(2)->setPosition(Vec2(938,812 - 106));
    labelsOutputs.at(3)->setPosition(Vec2(938,812 - 128));
    labelsOutputs.at(4)->setPosition(Vec2(938,812 - 150));
    labelsOutputs.at(5)->setPosition(Vec2(938,812 - 173));
    labelsOutputs.at(6)->setPosition(Vec2(938,812 - 217));

    dxLabel = Label::createWithTTF("0.00", "fonts/arial.ttf", 12);
    dxLabel->setTextColor(Color4B::BLACK);
    dxLabel->setAnchorPoint(Vec2(0,1));
    dxLabel->setPosition(Vec2(606,812 - 61));
    this->addChild(dxLabel);

    dyLabel = Label::createWithTTF("0.00", "fonts/arial.ttf", 12);
    dyLabel->setTextColor(Color4B::BLACK);
    dyLabel->setAnchorPoint(Vec2(0,1));
    dyLabel->setPosition(Vec2(606,812 - 87));
    this->addChild(dyLabel);

    this->schedule();
}

void ControlPanel::schedule(){
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(ControlPanel::update, this), this, 1.0f / 60, false, "controlPanel");
}

void ControlPanel::unschedule(){
    Director::getInstance()->getScheduler()->unschedule("controlPanel", this);
}

void ControlPanel::update(float dt){  
    if(AcademyFlappyBots::getInstance()->inferenceModeOn){
        for(int i = 0 ; i < 7; i++){
            float output = AcademyFlappyBots::getInstance()->inferenceBird->nn->currentOutputs.at(i);
            activeNeurons.at(i)->setVisible(output > 0.5);
            float val = output;
            stringstream ss (stringstream::in | stringstream::out);
            ss.precision(3);
            ss << val;
            string test = ss.str();
            labelsOutputs.at(i)->setString(test);
        }

        auto weights = AcademyFlappyBots::getInstance()->inferenceBird->nn->getWeightsAsVector();
        for(int i = 0 ; i < 12; i++){
            float val =  weights.at(i);;
            stringstream ss (stringstream::in | stringstream::out);
            ss.precision(3);
            ss << val;
            string test = ss.str();
            labelsWeights.at(i)->setString(test);
        }

        float val = AcademyFlappyBots::getInstance()->inferenceBird->dx;
        stringstream ss (stringstream::in | stringstream::out);
        ss.precision(3);
        ss << val;
        string dxStr = ss.str();
        dxLabel->setString(dxStr);

        val = AcademyFlappyBots::getInstance()->inferenceBird->dy;
        stringstream ss2 (stringstream::in | stringstream::out);
        ss2.precision(3);
        ss2 << val;
        string dyStr = ss2.str();
        dyLabel->setString(dyStr);
    }
}

void ControlPanel::startTrainingPressed(){
    log("startTrainingPressed pressed");
}
void ControlPanel::watchBestPlayerPressed(){
    log("watchBestPlayerPressed");
}

// template <typename T> string tostr(const T& t){ 
//    ostringstream os; 
//    os<<t; 
//    return os.str(); 
// } 

