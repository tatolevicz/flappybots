//
//  NeuralNetwork.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 28/11/19.
//

#ifndef NeuralNetwork_hpp
#define NeuralNetwork_hpp

#include <stdio.h>
#include "cocos2d.h"

class NeuralNetwork : cocos2d::Ref {
//    static NeuralNetwork* createNeuralNetwork();
public:
    virtual bool init();
    CREATE_FUNC(NeuralNetwork);
    
private:
    
};

#endif /* NeuralNetwork_hpp */
