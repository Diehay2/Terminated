#include "SDL_utils.h"
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
vector<Bullet*> enemy_bullet_list;
vector<Bullet*> player_bullet_list;

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


    const int EnemyNum = 30;
    vector<Enemy> enemy_list;
    for (int i = 0; i < EnemyNum; ++i) {
    Enemy enemy;
    enemy.p_player = &p_player;
    if (i < 5) {
        enemy.setPos(0 + i * 250, 0 + i * 2);
    }
    else if (i < 15) {
        enemy.setPos(200 + (i - 10) * 300, 280 + i * 3);
    }
    else if (i < 20) {
        enemy.setPos(200 + (i - 15) * 121, 224 + i * 2);
    }
    else if (i < 25) {
        enemy.setPos(0 + (i - 20) * 100 * 4.5, 540 - i * 7.6);
    }
    else {
        enemy.setPos(500 + (i - 25) * 400, 224);
    }
    enemy.load_enemy(renderer);
    enemy.setclip_standing();
    enemy.setclip_pistol();
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
        p_player.checkHit(enemy_bullet_list);

        for (Enemy& enemy : enemy_list) {
            enemy.DoEnemy(renderer, tileMap.getMap());
            enemy.checkHit(player_bullet_list);
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

