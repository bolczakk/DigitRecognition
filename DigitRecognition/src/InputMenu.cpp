#include "InputMenu.h"

InputMenu::InputMenu(const Vector2i& window_size) {
}

void InputMenu::drawMenu(SDL_Renderer* renderer, std::vector<float> image, TTF_Font* font) {
    drawNumber(renderer, image);
    drawText(renderer, font);
}

void InputMenu::drawNumber(SDL_Renderer* renderer, std::vector<float> image) {
    float x = 220.0f;
    float y = 100.0f;
    float scale = 12.0f;

    for (int i = 0; i < 28; ++i) {
        for (int j = 0; j < 28; ++j) {
            float pixel = image[i * 28 + j];
            SDL_SetRenderDrawColor(renderer, pixel * 255, pixel * 255, pixel * 255, 0);

            SDL_FRect rect;
            rect.x = x + j * scale;
            rect.y = y + i * scale;
            rect.w = scale;
            rect.h = scale;

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void InputMenu::drawInConsole(std::vector<float> image) {
    for (int i = 0; i < 28; ++i) {
        for (int j = 0; j < 28; ++j) {
            float pixel = image[i * 28 + j];
            if (pixel > 0.75f) {
                std::cout << "#";
            }
            else if (pixel > 0.5f) {
                std::cout << "+";
            }
            else if (pixel > 0.25f) {
                std::cout << ".";
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

const void InputMenu::drawText(SDL_Renderer* renderer, TTF_Font* font) {
    if (font == nullptr) {
        std::cerr << "Font not initialized!" << std::endl;
        return;
    }
    std::string ctx = "Click space to change input";
    SDL_Surface* textSurfaceValue = TTF_RenderText_Solid(font, ctx.c_str(), 0, textColor);
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
    textRectValue.x = 30.0f;
    textRectValue.y = 20.0f;
    textRectValue.w = textSurfaceValue->w;
    textRectValue.h = textSurfaceValue->h;
    SDL_RenderTexture(renderer, textTextureValue, nullptr, &textRectValue);

    SDL_DestroyTexture(textTextureValue);
    SDL_DestroySurface(textSurfaceValue);
}

void InputMenu::changeInput() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 10000);

    int random_number = dist(gen);

    this->current = random_number;
}
