#include "Circle.h"

Circle::Circle(Vector2f pos, SDL_Color color, float radius) : position(pos), radius(radius), color(color) {
}

const void Circle::SDL_CircleDraw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, ocolor.r, ocolor.g, ocolor.b, ocolor.a);
    float x = 0;
    float y = this->radius;
    float d = 1 - this->radius;

    while (x <= y) {
        SDL_RenderPoint(renderer, position.x + x, position.y + y);
        SDL_RenderPoint(renderer, position.x + y, position.y + x);
        SDL_RenderPoint(renderer, position.x - x, position.y + y);
        SDL_RenderPoint(renderer, position.x - y, position.y + x);
        SDL_RenderPoint(renderer, position.x + x, position.y - y);
        SDL_RenderPoint(renderer, position.x + y, position.y - x);
        SDL_RenderPoint(renderer, position.x - x, position.y - y);
        SDL_RenderPoint(renderer, position.x - y, position.y - x);

        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

const void Circle::SDL_CircleFilledDraw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    float x = 0;
    float y = this->radius;
    float d = 1 - this->radius;

    while (x <= y) {
        SDL_RenderLine(renderer, position.x - x, position.y + y, position.x + x, position.y + y);
        SDL_RenderLine(renderer, position.x - y, position.y + x, position.x + y, position.y + x);
        SDL_RenderLine(renderer, position.x - x, position.y - y, position.x + x, position.y - y);
        SDL_RenderLine(renderer, position.x - y, position.y - x, position.x + y, position.y - x);

        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

const Vector2f Circle::getPosition() const {
    return this->position;
}

const float Circle::getRadius() const {
    return radius;
}

void Circle::changeColor(SDL_Color color) {
    this->color = color;
}
