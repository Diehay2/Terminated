#include "SDL_utils.h"
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "object.h"
#include "menu.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
TTF_Font* font_button = nullptr;
TTF_Font* font_title = nullptr;

vector<Bullet*> player_bullet_list;
int killed = 0;
Object rifle;
Object bazoka;

int main(int argc, char* argv[]) {
    bool running = true;
    if (!Init()) return 1;

    Menu menu(window, renderer);
    MenuResult result = menu.run();
    if (result != MENU_PLAY) {
        quit();
        return 0;
    }

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
    p_player.setclip_rifle();
    p_player.setclip_bazoka();
    p_player.setclip_damaged();
    p_player.setclip_dead();
    p_player.setWeapon(0);

    Object rifle;
    rifle.setPos(850, 470);
    rifle.loadObject("Assets/Rifle1.png", renderer);

    Object bazoka;
    bazoka.setPos(530, 215);
    bazoka.loadObject("Assets/bazoka.png", renderer);

    const int EnemyNum = 1;

    vector<Enemy> enemy_list;
    for (int i = 0; i < EnemyNum; ++i) {
        Enemy enemy;
        enemy.p_player = &p_player;

        if (i > 5) enemy.setPos(0 + i * 250, 0 + i * 2);
        //else if (i < 10) enemy.setPos(118 + (i - 5) * 250, 280 + i * 3);
        //else if (i < 15) enemy.setPos(150 + (i - 10) * 210, 140 + i);
        //else if (i < 20) enemy.setPos(200 + (i - 15) * 121, 224 + i * 2);
        //else if (i < 25) enemy.setPos(0 + (i - 20) * 283, 540 - i * 7.6);
        //else if (i < 28) enemy.setPos(500 + (i - 25) * 350, 500 + (i - 20) * 2);
        else enemy.setPos(411 + (i - 0) * 250, 500 + (i - 20) * 2);

        if (i % 2 == 0) enemy.setWeapon();

        enemy.get_player_weapon(PISTOL);
        enemy.load_enemy(renderer);
        enemy.setclip_standing();
        enemy.setclip_pistol();
        enemy.setclip_rifle();
        enemy.setclip_damaged();
        enemy.setclip_dead();
        enemy_list.push_back(enemy);
    }

    bool paused = false;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (!paused) {
                p_player.handleInput(event, renderer);
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                paused = true;
                menu.setState(PAUSE_MENU);
                MenuResult pauseResult = menu.run();
                if (pauseResult == MENU_EXIT) {
                    running = false;
                } else if (pauseResult == MENU_PLAY) {
                    paused = false;
                }
            }
        }

        if (!paused) {
            p_player.DoPlayer(tileMap.getMap());
            int player_tile_x = static_cast<int>(p_player.get_x_pos()) / TILE_SIZE;
            int player_tile_y = static_cast<int>(p_player.get_y_pos()) / TILE_SIZE;

            int rifle_tile_x = static_cast<int>(rifle.get_x_pos()) / TILE_SIZE;
            int rifle_tile_y = static_cast<int>(rifle.get_y_pos()) / TILE_SIZE;

            int bazoka_tile_x = static_cast<int>(bazoka.get_x_pos()) / TILE_SIZE;
            int bazoka_tile_y = static_cast<int>(bazoka.get_y_pos()) / TILE_SIZE;

            if (player_tile_x == rifle_tile_x && player_tile_y == rifle_tile_y) {
                p_player.setWeapon(RIFLE);
                for (Enemy& enemy : enemy_list) enemy.get_player_weapon(RIFLE);
                rifle.free();
            }

            if (player_tile_x == bazoka_tile_x && player_tile_y == bazoka_tile_y) {
                p_player.setWeapon(BAZOKA);
                for (Enemy& enemy : enemy_list) enemy.get_player_weapon(BAZOKA);
                bazoka.free();
            }

            for (Enemy& enemy : enemy_list) {
                p_player.checkHit(enemy.getBulletlist());
                enemy.DoEnemy(renderer, tileMap.getMap());
                enemy.checkHit(player_bullet_list);
                if (enemy.is_dead() && !enemy.isCounted()) {
                    killed++;
                    enemy.setCounted(true);
                    cout << "Killed: " << killed << endl;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        tileMap.DrawMap(renderer);
        p_player.ShowObject(renderer, tileMap.getMap());
        rifle.render(renderer);
        bazoka.render(renderer);

        for (Enemy& enemy : enemy_list) {
            enemy.ShowObject(renderer, tileMap.getMap());
            enemy.renderHealthBar(renderer);
        }
        if (killed == EnemyNum) {
        menu.setState(WIN);
        MenuResult winResult = menu.run();
        killed = 0;
        }
        p_player.renderHealthBar(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }


    quit();
    return 0;
}

