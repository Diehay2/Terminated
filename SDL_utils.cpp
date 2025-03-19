#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Event event;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 640;
const char* TITLE = "Game";

void logError_Exit (const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Texture* loadImg(string file_path, SDL_Renderer* renderer) {

    SDL_Texture* texture = nullptr;
    texture = IMG_LoadTexture(renderer, file_path.c_str());

    if (texture == nullptr) {
            cout<<file_path<<" error: "<<IMG_GetError();
            return nullptr;
    }
    return texture;
}

bool Init() {

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
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

void quit(SDL_Window* window, SDL_Renderer* renderer) {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}



int main (int arc, char* argv[]) {
    bool running = true;
    if (Init() == false)
        return 0;

    SDL_Texture* background = loadImg("Assets/background.jpg",renderer);
     while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    quit(window,renderer);
    return 0;
}



