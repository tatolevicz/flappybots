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

class ControlPanel: public cocos2d::Node{
private:
    void initialSetup();
    void startTrainingPressed();
    void watchBestPlayerPressed();
    
    cocos2d::DrawNode* nnViewerPanel = nullptr;
    cocos2d::DrawNode* parametersViewerPanel = nullptr;
    cocos2d::DrawNode* initialPanel = nullptr;
    
protected:
    bool init() override;
    
public:
    CREATE_FUNC(ControlPanel);
};

#endif /*  ControlPanel_hpp */
