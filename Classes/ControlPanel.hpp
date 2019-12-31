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
    
    cocos2d::DrawNode* nnViewerPanel = nullptr;
    cocos2d::DrawNode* parametersViewerPanel = nullptr;
    cocos2d::DrawNode* initialPanel = nullptr;

    cocos2d::Sprite* base_nn = nullptr;
    cocos2d::Vector<cocos2d::Sprite*> activeNeurons;
    cocos2d::Vector<cocos2d::Label*> labelsOutputs;
    cocos2d::Vector<cocos2d::Label*> labelsWeights;
    cocos2d::Label* dxLabel = nullptr;
    cocos2d::Label* dyLabel = nullptr;
    
protected:
    bool init() override;
    
public:
    CREATE_FUNC(ControlPanel);
};

#endif /*  ControlPanel_hpp */
