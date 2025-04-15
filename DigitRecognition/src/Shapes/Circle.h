#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL3/SDL.h>
#include "../Vectors/Vectorxf.h"

class Circle {
public:
	Circle(Vector2f pos, SDL_Color color, float radius=20.0f);
	const void SDL_CircleDraw(SDL_Renderer* renderer) const;
	const void SDL_CircleFilledDraw(SDL_Renderer* renderer) const;
	const Vector2f getPosition() const;
	const float getRadius() const;
	void changeColor(SDL_Color color);
	SDL_Color color;
	SDL_Color ocolor = {255, 255, 255, 0};

private:
	float radius;
	//SDL_Color color;
	Vector2f position;
};

#endif
