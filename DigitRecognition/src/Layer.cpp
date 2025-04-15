#include "Layer.h"

Layer::Layer(int inputCount, int outputCount) {
    this->inputs.resize(inputCount);
    this->outputs.resize(outputCount);
    this->weights.resize(inputCount);
    this->biases.resize(outputCount);
    initWeightsBiases(inputCount, outputCount);
}

std::vector<float> Layer::feedForward(std::vector<float> givenInputs, Layer& layer) {
    for (int i = 0; i < layer.inputs.size(); i++) {
        layer.inputs[i] = givenInputs[i];
    }
    for (int i = 0; i < layer.outputs.size(); i++) {
        float sum = layer.biases[i];
        for (int j = 0; j < layer.inputs.size(); j++) {
            sum += layer.inputs[j] * layer.weights[j][i];
        }
        layer.outputs[i] = layer.sigmoid(sum);
    }
    return layer.outputs;
}

void Layer::initWeightsBiases(int inputCount, int outputCount) {
    for (int i = 0; i < inputCount; i++) {
        weights[i].resize(outputCount);
        for (int j = 0; j < outputCount; j++) {
            weights[i][j] = randomFloat();
        }
    }
    for (int i = 0; i < biases.size(); i++) {
        biases[i] = randomFloat();
    }
}

float Layer::randomFloat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    float randomFloat = dis(gen);

    return randomFloat;
}

float Layer::sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

float Layer::sigmoid_derivative(float x) {
    return x * (1 - x);
}

float Layer::RELU(float x) {
    return std::max(0.0f, x);
}

float Layer::RELU_derivative(float x) {
    return x > 0 ? 1.0f : 0.0f;
}

void Layer::saveLayer(std::ofstream& file) {
    for (const auto& bias : biases) {
        file.write(reinterpret_cast<const char*>(&bias), sizeof(bias));
    }
    for (const auto& weightRow : weights) {
        for (const auto& weight : weightRow) {
            file.write(reinterpret_cast<const char*>(&weight), sizeof(weight));
        }
    }
}

void Layer::loadLayer(std::ifstream& file) {
    for (auto& bias : biases) {
        file.read(reinterpret_cast<char*>(&bias), sizeof(bias));
    }
    for (auto& weightRow : weights) {
        for (auto& weight : weightRow) {
            file.read(reinterpret_cast<char*>(&weight), sizeof(weight));
        }
    }
}

std::vector<float> Layer::calculateError(const std::vector<float>& expectedOutputs) {
    std::vector<float> errors(expectedOutputs.size());
    for (size_t i = 0; i < outputs.size(); i++) {
        errors[i] = outputs[i] - expectedOutputs[i];
    }
    return errors;
}

void Layer::calculateOutputDeltas(const std::vector<float> error) {
    deltas.resize(outputs.size());
    for (int i = 0; i < deltas.size(); i++) {
        deltas[i] = 2 * error[i] * sigmoid_derivative(outputs[i]);
    }
}

void Layer::calculateHiddenDeltas(Layer& nextLayer) {
    deltas.resize(outputs.size());
    for (int i = 0; i < deltas.size(); i++) {
        deltas[i] = countSum(nextLayer, i) * sigmoid_derivative(outputs[i]);
    }
}

float Layer::countSum(Layer& nextLayer, int i) {
    float sum = 0.0f;
    for (size_t j = 0; j < nextLayer.outputs.size(); ++j) {
        sum += nextLayer.deltas[j] * nextLayer.weights[i][j];
    }
    return sum;
}

void Layer::updateWeightsAndBiases(float learningRate) {
    for (size_t i = 0; i < outputs.size(); i++) {
        biases[i] -= deltas[i] * learningRate;
        for (size_t j = 0; j < inputs.size(); j++) {
            weights[j][i] -= deltas[i] * inputs[j] * learningRate;
        }
    }
}
