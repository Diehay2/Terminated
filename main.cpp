#include "SDL_utils.h"

using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;



int main(int argc, char* argv[]) {
    bool running = true;
    if (!Init())
        return 1;

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

    quit();
    return 0;
}


