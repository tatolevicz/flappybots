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
    float out;
} NN_Neuron;

typedef struct layer{
    NN_Neuron* neurons;
    int numberOfNeurons;
} NN_Layer;

class NeuralNetwork : public  cocos2d::Ref {
private:
    NN_Layer inputLayer;
    NN_Layer* hiddenLayers;
    NN_Layer outputLayer;
    NN_Layer** layers;
    int numberOfHiddenLayers;
    int numberOfInputs;
    int numberOfOutputs;
    int* hiddenLayersSizes;
    int totalNumberOfLayers;
    int totalConnections;
    void setupNeuralNetwork();
    float activationFunction(float value);
    void allocNeurons(NN_Layer *layer, int size);
    void makeNeuronConnections(NN_Layer *layer, int numOfConnections);
    void setLayersReference();
    void setTotalConnections();
    void randomizeWeights();
public:
    CREATE_FUNC(NeuralNetwork);
    NeuralNetwork();
    ~NeuralNetwork();
    virtual bool init();
    virtual bool init(  int numberOfInputs,
                        int numberOfHiddenLayers,
                        int* hiddenLayersSizes,
                        int numberOfOutputs);
    std::vector<float> getOutput(std::vector<float> inputs);
    std::vector<float> getWeightsAsVector();
    void setWeightsFromVector(std::vector<float> newWeights);
    int getNumberOfConnections();
};
#endif /* NeuralNetwork_hpp */
