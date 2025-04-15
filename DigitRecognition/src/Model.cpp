#include "Model.h"

Model::Model(std::vector<int> layers, Start mode) : network(layers) {
	if (mode == LOAD) {
		testingData = std::make_unique<DataLoader>("data/testing/images.idx3-ubyte", "data/testing/labels.idx1-ubyte");
		try {
			std::cout << "Loaded testing data. " << testingData->images.size() << " images and " << testingData->labels.size() << " labels.\n";
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << "\n";
		}
		network.loadModel("model");
	}
	else if (mode == TRAIN) {
		DataLoader trainingData("data/training/images.idx3-ubyte", "data/training/labels.idx1-ubyte");
		try {
			std::cout << "Loaded training data. " << trainingData.images.size() << " images and " << trainingData.labels.size() << " labels.\n";
			std::cout << "<--------STARTING TRAINING-------->\n";
			std::cout << std::endl;
			//std::vector<std::vector<float>> new_vector_images(trainingData.images.begin(), trainingData.images.begin() + 1000);
			//std::vector<int> new_label(trainingData.labels.begin(), trainingData.labels.begin() + 1000);
			network.train(trainingData.images, trainingData.labels, epochs, learning_rate);
			network.saveModel("model");
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << "\n";
		}

		testingData = std::make_unique<DataLoader>("data/testing/images.idx3-ubyte", "data/testing/labels.idx1-ubyte");
		try {
			std::cout << "Loaded testing data. " << testingData->images.size() << " images and " << testingData->labels.size() << " labels.\n";
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << "\n";
		}
		network.loadModel("model");
	}
}
