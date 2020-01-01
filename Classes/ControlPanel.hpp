//
//  ControlPanel.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 29/12/19.
//
#ifndef ControlPanel_hpp
#define ControlPanel_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"


class ControlPanel: public cocos2d::Node{
private:
    void schedule();
    void unschedule();
    void update(float dt);
    void initialSetup();
    void startTrainingPressed();
    void watchBestPlayerPressed();
    
    std::string toStr(float val);

    cocos2d::DrawNode* nnViewerPanel = nullptr;
    cocos2d::DrawNode* parametersViewerPanel = nullptr;
    cocos2d::DrawNode* backGroundPanel = nullptr;
    cocos2d::DrawNode* initialPanel = nullptr;

    cocos2d::DrawNode* graphTrainingMode = nullptr;
    cocos2d::Vec2 lastPoint = cocos2d::Vec2::ZERO;

    cocos2d::Sprite* base_nn = nullptr;
    cocos2d::Sprite* base_parameters = nullptr;
    cocos2d::Vector<cocos2d::Sprite*> activeNeurons;
    cocos2d::Vector<cocos2d::Label*> labelsOutputs;
    cocos2d::Vector<cocos2d::Label*> labelsWeights;
    cocos2d::Vector<cocos2d::Label*> labelsOutputsNames;
    cocos2d::Vector<cocos2d::Label*> labelsWeightsNames;
    cocos2d::Label* dxLabel = nullptr;
    cocos2d::Label* dyLabel = nullptr;
    cocos2d::Label* dxLabelName = nullptr;
    cocos2d::Label* dyLabelName = nullptr;

    bool isActive = false;
    
protected:
    bool init() override;
    
public:
    CREATE_FUNC(ControlPanel);
    void setTrainingMode();
    void setInferenceMode();
    void setNoMode();
};

#endif /*  ControlPanel_hpp */
