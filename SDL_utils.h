#ifndef DSL_UTILS_H
#define DSL_UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const char* const TITLE = "Game";

bool Init();
void quit();
SDL_Texture* loadImg(const string& file_path, SDL_Renderer* renderer);
void logError_Exit(const char* msg, const char* error);

void logError_Exit (const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Texture* loadImg(const string& file_path, SDL_Renderer* renderer) {

    SDL_Texture* texture = nullptr;
    texture = IMG_LoadTexture(renderer, file_path.c_str());

    if (texture == nullptr) {
            cout<<file_path<<" error: "<<IMG_GetError();
            return nullptr;
    }
    return texture;
}

bool Init() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logError_Exit("SDL_Init",SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logError_Exit("CreateWindow",SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
        logError_Exit("CreateRenderer",SDL_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return true;
}

void quit() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

bool check_interaction(SDL_Rect a, SDL_Rect b) {
    return (
        a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y
        );
}

#endif // DSL_UTILS_H

