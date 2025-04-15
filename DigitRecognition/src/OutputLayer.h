#ifndef OUTPUTLAYER_H
#define OUTPUTLAYER_H

#include <vector>
#include <SDL3/SDL.h>
#include <iostream>
#include <random>

#include "Neuron.h"
#include "Vectors/Vectorxf.h"

class OutputLayer {
public:
	OutputLayer(const Vector2i& window_size, unsigned int num);
	const void drawContent(SDL_Renderer* renderer, TTF_Font* smallFont, TTF_Font* largeFont);
	void updateActivations(const std::vector<float> activations);
private:
	const float margin_y = 10.0f;
	const float margin_x = 150.0f;
	const float offset = 80.0f;

	float randomFloat();
	std::vector<Neuron> neurons;

	const unsigned int outputSize;
	const Vector2i window_size;
	const void drawNeurons(SDL_Renderer* renderer, TTF_Font* smallFont, TTF_Font* largeFont) const;
	const void drawResult(SDL_Renderer* renderer, TTF_Font* font);
	const SDL_Color textColor = { 255, 255, 255, 0 };

	int findResult();
	void initNeurons();
};

#endif