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

typedef struct neuron{
    float *weight;
    int numberOfConnections;
} Neuron;

typedef struct layer{
    Neuron* neurons;
    int numberOfNeurons;
} Layer;

class NeuralNetwork : cocos2d::Ref {
private:
    Layer inputLayer;
    Layer* hiddenLayers;
    Layer outputLayer;
    int numberOfHiddenLayers;
    int numberOfInputs;
    int numberOfOutputs;
    int* hiddenLayersSizes;
    void setupNeuralNetwork();
    

public:
    virtual bool init();
    virtual bool init(  int numberOfInputs,
                        int numberOfHiddenLayers,
                        int* hiddenLayersSizes,
                        int numberOfOutputs);
    CREATE_FUNC(NeuralNetwork);
    
private:
    
};

#endif /* NeuralNetwork_hpp */
