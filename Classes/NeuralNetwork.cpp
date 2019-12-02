//
//  NeuralNetwork.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 28/11/19.
//

#include "NeuralNetwork.hpp"

USING_NS_CC;

NeuralNetwork::NeuralNetwork(){
    //printf("Constructor NN called.\n")
;};
//TODO:: implement the destructor
NeuralNetwork::~NeuralNetwork(){
    printf("Destructor NN called.\n");
};

bool NeuralNetwork::init(){
    return true;
}

bool NeuralNetwork::init(int numberOfInputs,int numberOfHiddenLayers,int* hiddenLayersSizes,int numberOfOutputs){
    
    //do some simple check
    if(numberOfInputs < 1){
       log("Something went wrong: number of the inputs less than 1.");
       return false;
    }
    
    if(numberOfHiddenLayers < 1){
       log("Something went wrong: number the hiddenlayers lewss than 1.");
       return false;
    }
    
    if(numberOfOutputs < 1){
       log("Something went wrong: number the outputs less than 1.");
       return false;
    }
    
    this->numberOfInputs = numberOfInputs;
    this->numberOfHiddenLayers = numberOfHiddenLayers;
    this->numberOfOutputs = numberOfOutputs;
    this->hiddenLayersSizes = hiddenLayersSizes;
    this->totalNumberOfLayers = (this->numberOfHiddenLayers + 2);
    this->setupNeuralNetwork();
    return true;
}

void NeuralNetwork::setupNeuralNetwork(){
    //alloc input layer
    this->allocNeurons(&this->inputLayer,this->numberOfInputs);

    //alloc hidden layers 
    this->hiddenLayers = (NN_Layer*)malloc(sizeof(NN_Layer)*this->numberOfHiddenLayers);
    for(int i = 0; i< this->numberOfHiddenLayers; i++){
        this->allocNeurons(&this->hiddenLayers[i],this->hiddenLayersSizes[i]);
    }
    
    //alloc output layers
    this->allocNeurons(&this->outputLayer,this->numberOfOutputs);

    //add all layers to same ref to make it easier to calculate output
    this->setLayersReference();

    //set connections to all layers 
    for(int i = 1; i < (this->totalNumberOfLayers); i++){
        //layer connections
        this->makeNeuronConnections(this->layers[i],this->layers[i-1]->numberOfNeurons);
    }

    //initialize weights to all layers
    this->randomizeWeights();

    //initalize total connections variable
    this->setTotalConnections();
}

void NeuralNetwork::allocNeurons(NN_Layer *layer, int size){
    layer->neurons = (NN_Neuron*)malloc(sizeof(NN_Neuron)*size);
    layer->numberOfNeurons = size;
}

void NeuralNetwork::makeNeuronConnections(NN_Layer *layer, int numOfConnections){
    for(int j = 0; j <  layer->numberOfNeurons; j++){
        layer->neurons[j].numberOfConnections = numOfConnections;
        layer->neurons[j].weight = (float*)malloc(sizeof(float)*numOfConnections);
        for(int i = 0; i < numOfConnections; i++){
            layer->neurons[j].weight[i] = 1.0f;
        }
    }
}
void NeuralNetwork::setTotalConnections(){
    this->totalConnections = 0;
     for(int i = 1; i < this->totalNumberOfLayers; i++){
        for(int j = 0; j < this->layers[i]->numberOfNeurons; j++){
            for(int k = 0; k < this->layers[i]->neurons[j].numberOfConnections; k++){
                this->totalConnections++;
            }
        }
    }
}

void NeuralNetwork::setLayersReference(){
    this->layers = (NN_Layer**)malloc(sizeof(NN_Layer)*this->totalNumberOfLayers);
    this->layers[0] = &(this->inputLayer);
    for(int i = 0; i< this->numberOfHiddenLayers; i++){
        this->layers[i+1] = &this->hiddenLayers[i];
    }
    this->layers[this->totalNumberOfLayers-1] = &this->outputLayer;
}

void NeuralNetwork::randomizeWeights(){
    int contIdx = 0;
    std::vector<float> resp;
    resp.push_back(-0.94);
    resp.push_back(-0.87);
    resp.push_back(0.98);
    resp.push_back(-0.81);
    resp.push_back(0.47);
    resp.push_back(-0.76);
    resp.push_back(0.13);
    resp.push_back(0.68);
    resp.push_back(-0.26);
    resp.push_back(-0.12);
    resp.push_back(-0.57);
    resp.push_back(0.92);

    for(int i = 1; i < this->totalNumberOfLayers; i++){
        for(int j = 0; j < this->layers[i]->numberOfNeurons; j++){
            for(int k = 0; k < this->layers[i]->neurons[j].numberOfConnections; k++){
                this->layers[i]->neurons[j].weight[k] = ((float)rand())/(float)RAND_MAX;
                // this->layers[i]->neurons[j].weight[k] = resp.at(contIdx);
//                log("Number of time weghts: %d",k);
                contIdx++;
            }
        }
    }
}

std::vector<float> NeuralNetwork::getOutput(std::vector<float> inputs){
    //instance the output vector
    std::vector<float> outputs;
    outputs.reserve(this->numberOfOutputs);

    //just check if the number os inputs are ok
    if(inputs.size() != this->inputLayer.numberOfNeurons){
        log("Something went wrong: number the inputs is different.");
        return outputs;
    }
    
    //copy the inputs to the neural network
    for(int i = 0; i < this->inputLayer.numberOfNeurons; i++){
        this->inputLayer.neurons[i].out = inputs.at(i);
    }

    //do the feedfoward math
    for(int i = 1; i < this->totalNumberOfLayers; i++ ){
        for(int j = 0; j < this->layers[i]->numberOfNeurons; j++){
            float out = 0;
             for(int k = 0; k < this->layers[i]->neurons[j].numberOfConnections; k++ ){
                float input = this->layers[i-1]->neurons[k].out;
                float weight = this->layers[i]->neurons[j].weight[k];
                out += input * weight;
             }
            this->layers[i]->neurons[j].out = this->activationFunction(out);
        }
    }
    
    //copy the output from neuralnetwork to the output vector
    for(int i = 0; i < this->outputLayer.numberOfNeurons; i++){
        outputs.push_back(this->outputLayer.neurons[i].out);
    }

    return outputs;

}
//basic actiavtion function
//TODO: we should try another one like RELU
float NeuralNetwork::activationFunction(float value){
    float resp = 1/(1 + expf(-value));
    return resp;
}

std::vector<float> NeuralNetwork::getWeightsAsVector(){
    std::vector<float> resp;
    resp.reserve(this->totalConnections);

    for(int i = 1; i < this->totalNumberOfLayers; i++){
        for(int j = 0; j < this->layers[i]->numberOfNeurons; j++){
            for(int k = 0; k < this->layers[i]->neurons[j].numberOfConnections; k++){
                resp.push_back(this->layers[i]->neurons[j].weight[k]);
            }
        }
    }
    
    return resp;
}
void NeuralNetwork::setWeightsFromVector(std::vector<float> newWeights){
    int contIdx = 0;
    for(int i = 1; i < this->totalNumberOfLayers; i++){
        for(int j = 0; j < this->layers[i]->numberOfNeurons; j++){
            for(int k = 0; k < this->layers[i]->neurons[j].numberOfConnections; k++){
                this->layers[i]->neurons[j].weight[k] = newWeights.at(contIdx);
                contIdx++;
            }
        }
    }
}

int NeuralNetwork::getNumberOfConnections(){
    return this->totalConnections;
}
