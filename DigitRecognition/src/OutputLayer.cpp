#include "OutputLayer.h"

OutputLayer::OutputLayer(const Vector2i& window_size, unsigned int num) : outputSize(num), window_size(window_size) {
    initNeurons();
}

const void OutputLayer::drawContent(SDL_Renderer* renderer, TTF_Font* smallFont, TTF_Font* largeFont) {
    drawNeurons(renderer, smallFont, largeFont);
    drawResult(renderer, largeFont);
}

void OutputLayer::initNeurons() {
    const float layer_height = window_size.y - 2 * margin_y;
    const float center_y = margin_y + layer_height / 2.0f;

    for (unsigned int j = 0; j < outputSize; j++) {
        float y = (outputSize == 1) ? center_y : center_y + (j - (outputSize - 1) / 2.0f) * offset;
        neurons.emplace_back(Neuron(Vector2f(margin_x, y), j, randomFloat()));
    }
}

const void OutputLayer::drawNeurons(SDL_Renderer* renderer, TTF_Font* smallFont, TTF_Font* largeFont) const {
    for (auto& neuron : neurons) {
        neuron.SDL_CircleFilledDraw(renderer);
        neuron.SDL_CircleDraw(renderer);
        neuron.drawTextOutput(renderer, smallFont);
        neuron.drawTextValues(renderer, largeFont);
    }
}

const void OutputLayer::drawResult(SDL_Renderer* renderer, TTF_Font* font) {
    if (font == nullptr) {
        std::cerr << "Font not initialized!" << std::endl;
        return;
    }
    std::string ctx = "Expected output: ";
    SDL_Surface* textSurfaceValue = TTF_RenderText_Solid(font, (ctx + std::to_string(findResult())).c_str(), 0, textColor);
    if (!textSurfaceValue) {
        std::cerr << "Failed to create text surface: " << std::endl;
        return;
    }

    SDL_Texture* textTextureValue = SDL_CreateTextureFromSurface(renderer, textSurfaceValue);
    if (!textTextureValue) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(textSurfaceValue);
        return;
    }

    SDL_FRect textRectValue;
    textRectValue.x = static_cast<int>(400.0f);
    textRectValue.y = static_cast<int>(800.0f);
    textRectValue.w = float(textSurfaceValue->w);
    textRectValue.h = float(textSurfaceValue->h);
    SDL_RenderTexture(renderer, textTextureValue, nullptr, &textRectValue);

    SDL_DestroyTexture(textTextureValue);
    SDL_DestroySurface(textSurfaceValue);
}

int OutputLayer::findResult() {
    int max = 0;
    for (int i = 1; i < neurons.size(); i++) {
        if (neurons[i].activation > neurons[max].activation) {
            max = i;
        }
    }
    return max;
}

void OutputLayer::updateActivations(const std::vector<float> activations) {
    for (int i = 0; i < neurons.size(); i++) {
        neurons[i].updateActivation(activations[i]);
    }
}

float OutputLayer::randomFloat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    float randomFloat = dis(gen);

    return randomFloat;
}
