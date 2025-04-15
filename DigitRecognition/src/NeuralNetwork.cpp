#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(const std::vector<int>& layers) {
    if (layers.size() < 2) {
        throw std::invalid_argument("Layers vector must have at least 2 elements");
    }

    for (int i = 0; i < layers.size() - 1; i++) {
        this->layers.emplace_back(layers[i], layers[i+1]);
    }
}

std::vector<float> NeuralNetwork::forward(const std::vector<float>& givenInput) {
    std::vector<float> outputs = Layer::feedForward(givenInput, layers[0]);
    for (int i = 1; i < layers.size(); i++) {
        outputs = Layer::feedForward(outputs, layers[i]);
    }
    return outputs;
}

void NeuralNetwork::train(const std::vector<std::vector<float>>& inputs, const std::vector<int>& labels, int epochs, float learningRate) {
    Console::hideCursor();
    const size_t inputSize = inputs.size();

    for (int epoch = 0; epoch < epochs; epoch++) {
        float totalError = 0.0f;

        for (int i = 0; i < inputSize; i++) {
            std::vector<float> outputs = forward(inputs[i]);

            std::vector<float> expectedOutputs(layers.back().outputs.size(), 0.0f);
            expectedOutputs[labels[i]] = 1.0f;
            std::vector<float> errors = layers.back().calculateError(expectedOutputs);
            layers.back().calculateOutputDeltas(errors);

            for (int j = layers.size() - 2; j >= 0; j--) {
                layers[j].calculateHiddenDeltas(layers[j + 1]);
            }

            for (auto& layer : layers) {
                layer.updateWeightsAndBiases(learningRate);
            }

            for (float error : errors) {
                totalError += error * error;
            }
            Console::printProgressBar(i, inputSize);
        }

        std::cout << std::endl;
        std::cout << "Epoch " << epoch + 1 << " - Total Error: " << totalError/inputSize << std::endl;
        std::cout << std::endl;
    }
    Console::showCursor();
}

void NeuralNetwork::saveModel(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file to save model.");
    }
    for (auto& layer : layers) {
        layer.saveLayer(file);
    }
    file.close();
}

void NeuralNetwork::loadModel(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file to load model.");
    }
    for (auto& layer : layers) {
        layer.loadLayer(file);
    }
    file.close();
}
