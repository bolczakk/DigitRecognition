#include "Triangle.h"

Triangle::Triangle(std::vector<Vector2f> vertices_pos, SDL_Color color) : color(color) {
    SDL_zeroa(this->vertices);
    if (vertices_pos.size() == 3) {
        this->vertices[0].position.x = vertices_pos[0].x;
        this->vertices[0].position.y = vertices_pos[0].y;
        this->vertices[0].color.r = 1.0f;
        this->vertices[0].color.a = 1.0f;
        this->vertices[1].position.x = vertices_pos[1].x;
        this->vertices[1].position.y = vertices_pos[1].y;
        this->vertices[1].color.g = 1.0f;
        this->vertices[1].color.a = 1.0f;
        this->vertices[2].position.x = vertices_pos[2].x;
        this->vertices[2].position.y = vertices_pos[2].y;
        this->vertices[2].color.b = 1.0f;
        this->vertices[2].color.a = 1.0f;
    }
    else {
        std::cout << "Too many vertices! %s\n";
    }
}

const void Triangle::SDL_TriangleDraw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
}
