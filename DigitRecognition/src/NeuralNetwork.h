#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <cmath>
#include <random>
#include <iostream>

#include "Layer.h"
#include "Console.h"

class NeuralNetwork {
public:
	NeuralNetwork(const std::vector<int>& layers);

	std::vector<float> forward(const std::vector<float>& givenInput);
	void train(const std::vector<std::vector<float>>& inputs, const std::vector<int>& labels, int epochs, float learningRate);
	void saveModel(const std::string& filename);
	void loadModel(const std::string& filename);
private:
	std::vector<Layer> layers;
};


#endif