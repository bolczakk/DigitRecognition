#ifndef MODEL_H
#define MODEL_H

#include "DataLoader.h"
#include "NeuralNetwork.h"
#include "Console.h"

enum Start {
	TRAIN = 0,
	LOAD
};

class Model {
public:
	Model(std::vector<int> layers, Start mode);
	NeuralNetwork network;
	std::unique_ptr<DataLoader> testingData;
private:
	float learning_rate = 0.1f;
	int epochs = 6;
};

#endif