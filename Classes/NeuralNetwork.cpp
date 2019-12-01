//
//  NeuralNetwork.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 28/11/19.
//

#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(){printf("Constructor NN called.\n");};
NeuralNetwork::~NeuralNetwork(){printf("Destructor NN called.\n");};

bool NeuralNetwork::init(int numberOfInputs,int numberOfHiddenLayers,int* hiddenLayersSizes,int numberOfOutputs){
    this->numberOfInputs = numberOfInputs;
    this->numberOfHiddenLayers = numberOfHiddenLayers;
    this->numberOfOutputs = numberOfOutputs;
    this->hiddenLayersSizes = hiddenLayersSizes;
    this->setupNeuralNetwork();
    return true;
}

void NeuralNetwork::setupNeuralNetwork(){
    
    //alloc input layer
    this->inputLayer.neurons = (Neuron*)malloc(sizeof(Neuron)*this->numberOfInputs);
    this->inputLayer.numberOfNeurons = this->numberOfInputs;

    //alloc hidden layers
    this->hiddenLayers = (Layer*)malloc(sizeof(Layer)*this->numberOfHiddenLayers);
    for(int i = 0; i< this->numberOfHiddenLayers; i++){
        this->hiddenLayers[i].neurons = (Neuron*)malloc(sizeof(Neuron)*this->hiddenLayersSizes[i]);
        this->hiddenLayers[i].numberOfNeurons = this->hiddenLayersSizes[i];
    }

    //alloc output layers
    this->outputLayer.neurons = (Neuron*)malloc(sizeof(Neuron)*this->numberOfOutputs);
    this->outputLayer.numberOfNeurons = this->numberOfOutputs;
}