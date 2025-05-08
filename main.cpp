#include "SDL_utils.h"
#include "map.h"
#include "player.h"
#include "enemy.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;

using namespace std;

int main(int argc, char* argv[]) {
    bool running = true;
    if (!Init())
        return 1;

    SDL_Texture* background = loadImg("Assets/background1.PNG", renderer);

    TileMap tileMap;
    tileMap.loadMap("map.txt");
    tileMap.loadTiles(renderer);

    Player p_player;
    p_player.load_player(renderer);
    p_player.setclip_standing();
    p_player.setclip_walking();
    p_player.setclip_jumping();
    p_player.setclip_pistol();

    const int EnemyNum = 25;
    vector<Enemy> enemy_list;

    for (int i = 0; i < EnemyNum; ++i) {
        Enemy enemy;
        enemy.setPos(150 + i * 150, i * 20);
        enemy.load_enemy(renderer);
        enemy.setclip_standing();
        enemy_list.push_back(enemy);
    }

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            p_player.handleInput(event, renderer);
        }

        p_player.DoPlayer(tileMap.getMap());

        for (Enemy& enemy : enemy_list) {
            enemy.DoEnemy(tileMap.getMap());
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        tileMap.DrawMap(renderer);
        p_player.ShowObject(renderer, tileMap.getMap());

        for (Enemy& enemy : enemy_list) {
            enemy.ShowObject(renderer, tileMap.getMap());
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    quit();
    return 0;
}

