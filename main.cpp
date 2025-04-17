#include "SDL_utils.h"
#include "map.h"
#include "player.h"

using namespace std;

int main(int argc, char* argv[]) {
    bool running = true;
    if (!Init())
        return 1;

    SDL_Texture* background = loadImg("Assets/Capture.PNG",renderer);

    TileMap tileMap;
    tileMap.loadMap("map.txt");
    tileMap.loadTiles(renderer);

    Player p_player;
    p_player.load_player(renderer);
    p_player.setclip();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            p_player.handleInput(event,renderer);
        }

        p_player.DoPlayer(tileMap.getMap());

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        tileMap.DrawMap(renderer);
        p_player.ShowObject(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    quit();
    return 0;
}

