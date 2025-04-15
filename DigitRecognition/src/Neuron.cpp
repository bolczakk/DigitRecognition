#include "Neuron.h"

Neuron::Neuron(Vector2f pos, unsigned int digitValue, float activation, SDL_Color color, float radius) : Circle(pos, color, radius), activation(activation), digitValue(digitValue) {
    text = std::to_string(activation);
}

void Neuron::updateActivation(const float value) {
	this->activation = value;
	text = std::to_string(value);
	updateColor();
}

const void Neuron::drawTextOutput(SDL_Renderer* renderer, TTF_Font* font) const {
    if (font == nullptr) {
        std::cerr << "Font not initialized!" << std::endl;
        return;
    }

    SDL_Surface* textSurfaceOutput = TTF_RenderText_Solid(font, text.c_str(), 0, textColor);
    if (!textSurfaceOutput) {
        std::cerr << "Failed to create text surface: " << std::endl;
        return;
    }

    SDL_Texture* textTextureOutput = SDL_CreateTextureFromSurface(renderer, textSurfaceOutput);
    if (!textTextureOutput) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(textSurfaceOutput);
        return;
    }

    SDL_FRect textRectOutput;
    textRectOutput.x = getPosition().x + getRadius() + 18;
    textRectOutput.y = getPosition().y - getRadius() / 2;
    textRectOutput.w = float(textSurfaceOutput->w);
    textRectOutput.h = float(textSurfaceOutput->h);

    SDL_RenderTexture(renderer, textTextureOutput, nullptr, &textRectOutput);

    SDL_DestroyTexture(textTextureOutput);
    SDL_DestroySurface(textSurfaceOutput);
}

const void Neuron::drawTextValues(SDL_Renderer* renderer, TTF_Font* font) const {
    if (font == nullptr) {
        std::cerr << "Font not initialized!" << std::endl;
        return;
    }

    SDL_Surface* textSurfaceValue = TTF_RenderText_Solid(font, std::to_string(digitValue).c_str(), 0, textColor);
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
    textRectValue.x = getPosition().x + getRadius() - 90.0f;
    textRectValue.y = getPosition().y - getRadius() / 2.0f - 6.0f;
    textRectValue.w = float(textSurfaceValue->w);
    textRectValue.h = float(textSurfaceValue->h);
    SDL_RenderTexture(renderer, textTextureValue, nullptr, &textRectValue);

    SDL_DestroyTexture(textTextureValue);
    SDL_DestroySurface(textSurfaceValue);
}

float Neuron::randomFloat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    float randomFloat = dis(gen);

    return randomFloat;
}

void Neuron::updateColor() {
    Uint8 p = activation * 255.0f;
	this->color.r = p;
    this->color.g = p;
    this->color.b = p;
	this->color.a = 0;
}
