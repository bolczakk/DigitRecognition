#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>

#include "OutputLayer.h"
#include "Model.h"
#include "InputMenu.h"

static SDL_Window* networkWindow = NULL;
static SDL_Window* mainWindow = NULL;

static SDL_Renderer* networkRenderer = NULL;
static SDL_Renderer* mainRenderer = NULL;

static SDL_Texture* texture = NULL;
static TTF_Font* smallFont = NULL;
static TTF_Font* largeFont = NULL;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT_NETWORK = 900;
constexpr int WINDOW_HEIGHT = 600;

Vector2i window_size_n = Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT_NETWORK);
Vector2i window_size_m = Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT);

OutputLayer output(window_size_n, 10);
InputMenu menu(window_size_m);

std::vector<float> result;

Start mode = LOAD; //epochs, learning rate in model.h
std::vector<int> layers = { 784, 128, 10 };

Model model(layers, mode);

void SDL_AppQuit(void* appstate, SDL_AppResult result);

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    result = model.network.forward(model.testingData->images[menu.current]);
    std::cout << "Forwarding first image, label: " << model.testingData->labels[menu.current] << "\n";

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("Input Window", WINDOW_WIDTH, WINDOW_HEIGHT, NULL, &mainWindow, &mainRenderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("Network Output", WINDOW_WIDTH, WINDOW_HEIGHT_NETWORK, NULL, &networkWindow, &networkRenderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    smallFont = TTF_OpenFont("assets/fonts/CascadiaCode.ttf", 18);
    largeFont = TTF_OpenFont("assets/fonts/CascadiaCode.ttf", 28);
    if (!smallFont || !largeFont) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Surface* icon = SDL_LoadBMP("assets/icon.bmp");
    if (!icon) {
        printf("Couldn't load text: %s\n", SDL_GetError());
    }
    else {
        SDL_SetWindowIcon(networkWindow, icon);
        SDL_SetWindowIcon(mainWindow, icon);
        SDL_DestroySurface(icon);
    }

    SDL_SetWindowPosition(networkWindow, 100, 100);
    SDL_SetWindowPosition(mainWindow, 950, 100);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_SPACE) {
            menu.changeInput();
            result = model.network.forward(model.testingData->images[menu.current]);
        }
    }
    return SDL_APP_CONTINUE;
}

const Uint32 frameDelay = 1000 / 120;
Uint64 frameStart;
Uint64 frameTime;

SDL_AppResult SDL_AppIterate(void* appstate) {
    frameStart = SDL_GetTicks();

    SDL_SetRenderDrawColor(networkRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(networkRenderer);
    SDL_RenderClear(mainRenderer);

    output.updateActivations(result);
    output.drawContent(networkRenderer, smallFont, largeFont);

    menu.drawMenu(mainRenderer, model.testingData->images[menu.current], largeFont);

    SDL_RenderPresent(networkRenderer);
    SDL_RenderPresent(mainRenderer);

    frameTime = SDL_GetTicks() - frameStart;

    if (frameDelay > frameTime) {
        SDL_Delay(frameDelay - frameTime);
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    /*if (font) {
        TTF_CloseFont(font);
    }
    TTF_CloseFont(font); <- causes lag*/
}