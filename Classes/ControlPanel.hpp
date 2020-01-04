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
#include "GameManager.hpp"


class ControlPanel: public cocos2d::Node{
private:
    static ControlPanel* instance;

    float graphScaleX = 6.15;
    float graphScaleY = 0.20 * GameManager::getInstance()->worldSpeed;

    float graphOffSetX = 30;
    float graphOffSetY = 31;

    float timePlot = 0;

    void schedule();
    void unschedule();
   void update(float dt);
    void initialSetup();
    void startTrainingPressed();
    void watchBestPlayerPressed();
    void drawLineTo(cocos2d::Vec2 dest);
    
    std::string toStr(float val);

    cocos2d::DrawNode* nnViewerPanel = nullptr;
    cocos2d::DrawNode* parametersViewerPanel = nullptr;
    cocos2d::DrawNode* backGroundPanel = nullptr;
    cocos2d::DrawNode* initialPanel = nullptr;

    cocos2d::DrawNode* graphTrainingMode = nullptr;
    cocos2d::Vec2 lastPoint = cocos2d::Vec2(35,33);

    cocos2d::Sprite* base_nn = nullptr;
    cocos2d::Sprite* graphBase = nullptr;
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

    void plot();
    static ControlPanel* getInstance();
};


#endif /*  ControlPanel_hpp */
