#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Vectors/Vectorxf.h"
#include <SDL3/SDL.h>
#include <vector>
#include <iostream>

class Triangle {
public:
	Triangle(std::vector<Vector2f> vertices_pos, SDL_Color color);
	const void SDL_TriangleDraw(SDL_Renderer* renderer) const;
private:
	SDL_Vertex vertices[3];
	SDL_Color color;
};

#endif

