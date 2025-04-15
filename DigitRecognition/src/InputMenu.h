#ifndef INPUTMENU_H
#define INPUTMENU_H

#include "Vectors/Vectorxf.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <random>
#include <iostream>

class InputMenu {
public:
	InputMenu(const Vector2i& window_size);
	void drawMenu(SDL_Renderer* renderer, std::vector<float> image, TTF_Font* font);
	void changeInput();
	int current = 0;
private:
	const SDL_Color textColor = { 255, 255, 255, 0 };
	void drawNumber(SDL_Renderer* renderer, std::vector<float> image);
	void drawInConsole(std::vector<float> image);

	const void drawText(SDL_Renderer* renderer, TTF_Font* font);
};

#endif
