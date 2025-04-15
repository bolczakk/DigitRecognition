#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <random>
#include <fstream>
#include <cuda_runtime.h>
#include <iostream>

class Layer {
public:
	Layer(int inputCount, int outputCount);
	std::vector<float> outputs;
	std::vector<float> calculateError(const std::vector<float>& expectedOutputs);
	void calculateOutputDeltas(const std::vector<float> error);
	void calculateHiddenDeltas(Layer& nextLayer);
	float countSum(Layer& nextLayer, int i);
	void updateWeightsAndBiases(float learningRate);

	static std::vector<float> feedForward(std::vector<float> givenInputs, Layer& layer);

	void saveLayer(std::ofstream& file);
	void loadLayer(std::ifstream& file);

private:
	std::vector<float> inputs;
	std::vector<float> biases;
	std::vector<std::vector<float>> weights;
	std::vector<float> deltas;

	void initWeightsBiases(int inputCount, int outputCount);
	float randomFloat();
	float sigmoid(float x);
	float sigmoid_derivative(float x);

	float RELU(float x);
	float RELU_derivative(float x);
};

#endif // !LAYER_H
