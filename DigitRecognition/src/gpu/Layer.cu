#include "Layer.cuh"

Layer::Layer(int inputCount, int outputCount) {
    this->inputs.resize(inputCount);
    this->outputs.resize(outputCount);
    this->weights.resize(inputCount);
    this->biases.resize(outputCount);
    initWeightsBiases(inputCount, outputCount);
}

__device__ float sigmoid(float x) {
    return 1.0f / (1.0f + exp(-x));
}

// CUDA kernel dla obliczeñ wyjœciowych
__global__ void computeOutputs(
    float* d_inputs, float* d_weights, float* d_biases, float* d_outputs,
    int inputSize, int outputSize) {
    int outputIdx = blockIdx.x * blockDim.x + threadIdx.x;

    if (outputIdx < outputSize) {
        float sum = d_biases[outputIdx];
        for (int i = 0; i < inputSize; i++) {
            sum += d_inputs[i] * d_weights[i * outputSize + outputIdx];
        }
        d_outputs[outputIdx] = sigmoid(sum);
    }
}

std::vector<float> Layer::feedForward(std::vector<float> givenInputs, Layer& layer) {
    int inputSize = layer.inputs.size();
    int outputSize = layer.outputs.size();
    float* d_inputs;
    float* d_weights;
    float* d_biases;
    float* d_outputs;

    for (int i = 0; i < layer.inputs.size(); i++) {
        layer.inputs[i] = givenInputs[i];
    }

    // Rozmiar wag w postaci 1D (inputSize * outputSize)
    size_t weightsSize = inputSize * outputSize * sizeof(float);
    size_t inputsSize = inputSize * sizeof(float);
    size_t biasesSize = outputSize * sizeof(float);
    size_t outputsSize = outputSize * sizeof(float);

    cudaMalloc(&d_inputs, inputsSize);
    cudaMalloc(&d_weights, weightsSize);
    cudaMalloc(&d_biases, biasesSize);
    cudaMalloc(&d_outputs, outputsSize);

    // Przekszta³cenie wag do 1D
    std::vector<float> flatWeights(inputSize * outputSize);
    for (int i = 0; i < inputSize; i++) {
        for (int j = 0; j < outputSize; j++) {
            flatWeights[i * outputSize + j] = layer.weights[i][j];
        }
    }

    // Kopiowanie danych z hosta (CPU) do urz¹dzenia (GPU)
    cudaMemcpy(d_inputs, layer.inputs.data(), inputsSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_weights, flatWeights.data(), weightsSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_biases, layer.biases.data(), biasesSize, cudaMemcpyHostToDevice);

    // Obliczenia na GPU: konfiguracja siatki i bloków
    int threadsPerBlock = 256;
    int blocksPerGrid = (outputSize + threadsPerBlock - 1) / threadsPerBlock;

    computeOutputs << <blocksPerGrid, threadsPerBlock >> > (
        d_inputs, d_weights, d_biases, d_outputs,
        inputSize, outputSize);

    // Pobranie wyników z GPU
    std::vector<float> outputs(outputSize);
    cudaMemcpy(outputs.data(), d_outputs, outputsSize, cudaMemcpyDeviceToHost);

    // Zwolnienie pamiêci GPU
    cudaFree(d_inputs);
    cudaFree(d_weights);
    cudaFree(d_biases);
    cudaFree(d_outputs);

    return outputs;
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

__global__ void calculateHiddenDeltasKernel(
    float* deltas,
    const float* outputs,
    const float* nextDeltas,
    const float* nextWeights,
    int outputSize,
    int nextOutputSize) {

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < outputSize) {
        float sum = 0.0f;
        for (int j = 0; j < nextOutputSize; ++j) {
            sum += nextDeltas[j] * nextWeights[idx * nextOutputSize + j];
        }
        deltas[idx] = sum * outputs[idx] * (1 - outputs[idx]); // sigmoid_derivative
    }
}


void Layer::calculateHiddenDeltas(Layer& nextLayer) {
    int outputSize = outputs.size();
    int nextOutputSize = nextLayer.outputs.size();

    // Alokacja pamiêci na GPU
    float* d_deltas, * d_outputs, * d_nextDeltas, * d_nextWeights;

    cudaMalloc(&d_deltas, outputSize * sizeof(float));
    cudaMalloc(&d_outputs, outputSize * sizeof(float));
    cudaMalloc(&d_nextDeltas, nextOutputSize * sizeof(float));
    cudaMalloc(&d_nextWeights, outputSize * nextOutputSize * sizeof(float));

    // Kopiowanie danych na GPU
    cudaMemcpy(d_outputs, outputs.data(), outputSize * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_nextDeltas, nextLayer.deltas.data(), nextOutputSize * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_nextWeights, nextLayer.weights[0].data(),
        outputSize * nextOutputSize * sizeof(float), cudaMemcpyHostToDevice);

    // Wywo³anie kernela
    int threadsPerBlock = 256;
    int blocksPerGrid = (outputSize + threadsPerBlock - 1) / threadsPerBlock;
    calculateHiddenDeltasKernel << <blocksPerGrid, threadsPerBlock >> > (
        d_deltas, d_outputs, d_nextDeltas, d_nextWeights, outputSize, nextOutputSize);

    // Kopiowanie wyników z GPU do pamiêci hosta
    cudaMemcpy(deltas.data(), d_deltas, outputSize * sizeof(float), cudaMemcpyDeviceToHost);

    // Zwolnienie pamiêci GPU
    cudaFree(d_deltas);
    cudaFree(d_outputs);
    cudaFree(d_nextDeltas);
    cudaFree(d_nextWeights);
}

float Layer::countSum(Layer& nextLayer, int i) {
    float sum = 0.0f;
    for (size_t j = 0; j < nextLayer.outputs.size(); ++j) {
        sum += nextLayer.deltas[j] * nextLayer.weights[i][j];
    }
    return sum;
}

__global__ void updateBiases(float* d_biases, float* d_deltas, float learningRate, int outputSize) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < outputSize) {
        d_biases[idx] -= d_deltas[idx] * learningRate;
    }
}

// CUDA kernel do aktualizacji wag
__global__ void updateWeights(
    float* d_weights, float* d_inputs, float* d_deltas,
    float learningRate, int inputSize, int outputSize) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    int row = idx / outputSize;  // Indeks wiersza (wejœcie)
    int col = idx % outputSize;  // Indeks kolumny (wyjœcie)

    if (row < inputSize && col < outputSize) {
        int weightIdx = row * outputSize + col;
        d_weights[weightIdx] -= d_deltas[col] * d_inputs[row] * learningRate;
    }
}

void Layer::updateWeightsAndBiases(float learningRate) {
    int inputSize = inputs.size();
    int outputSize = biases.size();

    // Alokacja pamiêci na GPU
    float* d_biases;
    float* d_deltas;
    float* d_weights;
    float* d_inputs;

    size_t biasesSize = outputSize * sizeof(float);
    size_t deltasSize = outputSize * sizeof(float);
    size_t weightsSize = inputSize * outputSize * sizeof(float);
    size_t inputsSize = inputSize * sizeof(float);

    cudaMalloc(&d_biases, biasesSize);
    cudaMalloc(&d_deltas, deltasSize);
    cudaMalloc(&d_weights, weightsSize);
    cudaMalloc(&d_inputs, inputsSize);

    // Przekszta³cenie wag do 1D
    std::vector<float> flatWeights(inputSize * outputSize);
    for (int i = 0; i < inputSize; i++) {
        for (int j = 0; j < outputSize; j++) {
            flatWeights[i * outputSize + j] = weights[i][j];
        }
    }

    // Kopiowanie danych z CPU na GPU
    cudaMemcpy(d_biases, biases.data(), biasesSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_deltas, deltas.data(), deltasSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_weights, flatWeights.data(), weightsSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_inputs, inputs.data(), inputsSize, cudaMemcpyHostToDevice);

    // Konfiguracja siatki i bloków
    int threadsPerBlock = 256;
    int biasBlocks = (outputSize + threadsPerBlock - 1) / threadsPerBlock;
    int weightBlocks = ((inputSize * outputSize) + threadsPerBlock - 1) / threadsPerBlock;

    // Uruchomienie kernelów
    updateBiases << <biasBlocks, threadsPerBlock >> > (d_biases, d_deltas, learningRate, outputSize);
    updateWeights << <weightBlocks, threadsPerBlock >> > (
        d_weights, d_inputs, d_deltas, learningRate, inputSize, outputSize);

    // Pobranie zaktualizowanych danych z GPU na CPU
    cudaMemcpy(biases.data(), d_biases, biasesSize, cudaMemcpyDeviceToHost);

    cudaMemcpy(flatWeights.data(), d_weights, weightsSize, cudaMemcpyDeviceToHost);

    // Przekszta³cenie wag z 1D na 2D
    for (int i = 0; i < inputSize; i++) {
        for (int j = 0; j < outputSize; j++) {
            weights[i][j] = flatWeights[i * outputSize + j];
        }
    }

    // Zwolnienie pamiêci na GPU
    cudaFree(d_biases);
    cudaFree(d_deltas);
    cudaFree(d_weights);
    cudaFree(d_inputs);
}
