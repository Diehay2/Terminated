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
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;
extern TTF_Font* font_button;
extern TTF_Font* font_title;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const char* const TITLE = "Nobodies";

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



    if (TTF_Init() == -1) {
        logError_Exit("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());
        return false;
    }
    font_button = TTF_OpenFont("Assets/Font/CyberpunkCraftpixPixel.otf", 28);
    font_title = TTF_OpenFont("Assets/Font/CyberpunkCraftpixPixel.otf", 80);
    if (!font_button || !font_title) {
        logError_Exit("LoadFont", TTF_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return true;
}

    void renderText(const string& text, int x, int y, TTF_Font* font) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), white);
        if (!surface) return;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

void quit() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (font_button && font_title) {
    TTF_CloseFont(font_button);
    TTF_CloseFont(font_title);
    font_button = nullptr;
    font_title = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool check_interaction(const SDL_Rect& a, const SDL_Rect& b) {
    return !(a.x + a.w - 15 <= b.x || a.x + 15 >= b.x + b.w ||
             a.y + a.h <= b.y || a.y >= b.y + b.h);
}
#endif // DSL_UTILS_H

