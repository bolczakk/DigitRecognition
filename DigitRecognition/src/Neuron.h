#ifndef NEURON_H
#define NEURON_H

#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <iostream>
#include <random>

#include "Shapes/Circle.h"

class Neuron : public Circle {
public:
	Neuron(Vector2f pos, unsigned int digitValue, float activation, SDL_Color color =  {255, 255, 255, 0}, float radius = 20.0f);
	float activation = 1.0f;
	void updateActivation(const float value);
	const void drawTextOutput(SDL_Renderer* renderer, TTF_Font* font) const;
	const void drawTextValues(SDL_Renderer* renderer, TTF_Font* font) const;
private:
	std::string text;
	float randomFloat();
	void updateColor();
	unsigned int digitValue;
	SDL_Color textColor = { 255, 255, 255, 255 };
};

#endif
