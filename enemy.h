#ifndef ENEMY_H
#define ENEMY_H

#include "SDL_utils.h"
#include "map.h"
#include "player.h"
#include "bullet.h"

class Enemy {
public:
    Enemy() {
        rect.x = 0;
        rect.y = 0;
        rect.w = 0;
        rect.h = 0;
    }
    ~Enemy() {;}
    Player* p_player;

    void setPos(float x, float y) {
        x_pos = x;
        y_pos = y;

    }

    bool load_enemy(SDL_Renderer* screen) {
    texture_standing_left = loadImg("Assets/animations/enemy_standing_left.png", screen);
    texture_standing_right = loadImg("Assets/animations/enemy_standing_right.png", screen);

    texture_pistol_left = loadImg("Assets/animations/enemy_pistol_left.png", screen);
    texture_pistol_right = loadImg("Assets/animations/enemy_pistol_right.png", screen);

    texture_rifle_left = loadImg("Assets/animations/enemy_rifle_left.png", screen);
    texture_rifle_right = loadImg("Assets/animations/enemy_rifle_right.png", screen);

    texture_damaged_left = loadImg("Assets/animations/enemy_damaged_left.png", screen);
    texture_damaged_right = loadImg("Assets/animations/enemy_damaged_right.png",screen);

    texture_dead_left = loadImg("Assets/animations/enemy_dead_left.png", screen);
    texture_dead_right = loadImg("Assets/animations/enemy_dead_right.png", screen);

    if (texture_standing_left == nullptr || texture_standing_right == nullptr
        || texture_pistol_left == nullptr || texture_pistol_right == nullptr
        || texture_damaged_left == nullptr || texture_damaged_right == nullptr
        || texture_rifle_left == nullptr || texture_rifle_right == nullptr
        || texture_dead_left == nullptr || texture_dead_right == nullptr) {
        logError_Exit("Load Enemy", SDL_GetError());
        return false;
    }
    SDL_QueryTexture(texture_standing_right, nullptr, nullptr, &rect.w, &rect.h);
    stand_width_frame = rect.w / 3;
    stand_height_frame = rect.h;

    SDL_QueryTexture(texture_pistol_right, nullptr, nullptr, &rect.w, &rect.h);
    pistol_width_frame = rect.w;
    pistol_height_frame = rect.h;

    SDL_QueryTexture(texture_rifle_right, nullptr, nullptr, &rect.w, &rect.h);
    rifle_width_frame = rect.w;
    rifle_height_frame = rect.h;

    SDL_QueryTexture(texture_damaged_right, nullptr, nullptr, &rect.w, &rect.h);
    damaged_width_frame = rect.w;
    damaged_height_frame = rect.h;

    SDL_QueryTexture(texture_dead_right, nullptr, nullptr, &rect.w, &rect.h);
    dead_width_frame = rect.w;
    dead_height_frame = rect.h;
    return true;
}

    void setclip_standing() {
    if (stand_width_frame > 0 && stand_height_frame > 0) {
            for (int i = 0; i < 3; i++) {
        frame_clip_standing[i].x = i * stand_width_frame;
        frame_clip_standing[i].y = 0;
        frame_clip_standing[i].w = stand_width_frame;
        frame_clip_standing[i].h = stand_height_frame;
            }
        }
    }

    void setclip_pistol() {
    if (pistol_width_frame > 0 && pistol_height_frame > 0) {
        frame_clip_pistol[0].x = 0;
        frame_clip_pistol[0].y = 0;
        frame_clip_pistol[0].w = pistol_width_frame;
        frame_clip_pistol[0].h = pistol_height_frame;
        }
    }

    void setclip_rifle() {
    if (rifle_width_frame > 0 && rifle_height_frame > 0) {
        frame_clip_rifle[0].x = 0;
        frame_clip_rifle[0].y = 0;
        frame_clip_rifle[0].w = rifle_width_frame;
        frame_clip_rifle[0].h = rifle_height_frame;
        }
    }

    void setclip_damaged() {
    if (damaged_width_frame > 0 && damaged_height_frame > 0) {
        frame_clip_damaged[0].x = 0;
        frame_clip_damaged[0].y = 0;
        frame_clip_damaged[0].w = damaged_width_frame;
        frame_clip_damaged[0].h = damaged_height_frame;
        }
    }

    void setclip_dead() {
    if (dead_width_frame > 0 && dead_height_frame > 0) {
        frame_clip_dead[0].x = 0;
        frame_clip_dead[0].y = 0;
        frame_clip_dead[0].w = dead_width_frame;
        frame_clip_dead[0].h = dead_height_frame;
        }
    }

    void ShowObject(SDL_Renderer* des, Map& map_data) {
    SDL_Rect* current_clip;
    SDL_Texture* current_texture;

    if (status == PISTOL_LEFT || status == PISTOL_RIGHT) {
    frame = 0;
    current_clip = &frame_clip_pistol[frame];
    current_texture = (status == PISTOL_LEFT || status == STANDING_LEFT)
                    ? texture_pistol_left : texture_pistol_right;
    width_frame = pistol_width_frame;
    height_frame = pistol_height_frame;
    }

    else if (status == DAMAGED_LEFT || status == DAMAGED_RIGHT) {
    frame = 0;
    current_clip = &frame_clip_damaged[frame];
    current_texture = (status == DAMAGED_LEFT) ? texture_damaged_left : texture_damaged_right;
    width_frame = damaged_width_frame;
    height_frame = damaged_height_frame;
    }

    else if (status == RIFLE_LEFT || status == RIFLE_RIGHT) {
    frame = 0;
    is_shooting = false;
    current_clip = &frame_clip_rifle[frame];
    current_texture = (status == RIFLE_LEFT || status == STANDING_LEFT)
                    ? texture_rifle_left : texture_rifle_right;
    width_frame = rifle_width_frame;
    height_frame = rifle_height_frame;
    }

    else if (status == DEAD_LEFT || status == DEAD_RIGHT) {
    frame = 0;
    current_clip = &frame_clip_dead[frame];
    current_texture = (status == DEAD_LEFT) ? texture_dead_left : texture_dead_right;
    width_frame = dead_width_frame;
    height_frame = dead_height_frame;
    }


    else {
        animation_delay_counter++;
        if (animation_delay_counter >= animation_delay_max) {
            frame++;
            if (frame >= 3) frame = 0;
            animation_delay_counter = 0;
        }
        current_clip = &frame_clip_standing[frame];
        current_texture = (status == STANDING_LEFT) ? texture_standing_left : texture_standing_right;
        width_frame = stand_width_frame;
        height_frame = stand_height_frame;
        }

    SDL_Rect renderQuad;
    if (status == DEAD_LEFT || status == DEAD_RIGHT) renderQuad = {static_cast<int>(x_pos), static_cast<int>(y_pos + 5), width_frame, height_frame};
    else if (status == DAMAGED_LEFT || status == DAMAGED_RIGHT) renderQuad = {static_cast<int>(x_pos), static_cast<int>(y_pos + 4), width_frame, height_frame};
    else if (status == STANDING_LEFT || status == STANDING_RIGHT) renderQuad = {static_cast<int>(x_pos), static_cast<int>(y_pos + 3), width_frame, height_frame};
    else renderQuad = {static_cast<int>(x_pos), static_cast<int>(y_pos + 1), width_frame, height_frame};
    SDL_RenderCopy(des, current_texture, current_clip, &renderQuad);

    {for (int i = 0; i < bullet_list.size(); ++i) {
    Bullet* bullet = bullet_list[i];
    if (bullet != nullptr) {
        bullet->fire();
        bullet->updateFrame();
        bullet->render(des);
        if (bullet->OutRange() || check_to_bullet(bullet, map_data)) {
            delete bullet;
            bullet_list.erase(bullet_list.begin() + i);
            i--;
        }
    }
    }
}

    }

    bool check_to_bullet(Bullet* bullet, Map& map_data) {
    SDL_Rect rect = bullet->getRect();

    int left_tile = rect.x / TILE_SIZE;
    int right_tile = (rect.x + rect.w - 1) / TILE_SIZE;
    int top_tile = rect.y / TILE_SIZE;
    int bottom_tile = (rect.y + rect.h - 1) / TILE_SIZE;

    if (left_tile < 0 || right_tile >= MAX_MAP_X || top_tile < 0 || bottom_tile >= MAX_MAP_Y) return false;

    for (int y = top_tile; y <= bottom_tile; ++y) {
        for (int x = left_tile; x <= right_tile; ++x) {
            if (map_data.tile[y][x] != 0) return true;
        }
    }

    return false;
}

    void check_to_map(Map& map_data) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;
    y1 = (y_pos + 1) / TILE_SIZE;
    y2 = (y_pos + height_frame - 1) / TILE_SIZE;

    if (x_val > 0) {
        if (map_data.tile[y1][x2] > 0 || map_data.tile[y2][x2] > 0) {
            x_pos = x2 * TILE_SIZE - width_frame;
            x_val = 0;
        }
    }
    else if (x_val < 0) {
        if (map_data.tile[y1][x1] > 0 || map_data.tile[y2][x1] > 0) {
            x_pos = (x1 + 1) * TILE_SIZE;
            x_val = 0;
        }
    }

    x_pos += x_val;
    y_pos += y_val;

    int height_min = min(height_frame,TILE_SIZE);
    x1 = (x_pos + 1) / TILE_SIZE;
    x2 = (x_pos + width_frame - 1) / TILE_SIZE;
    y1 = (y_pos + y_val) / TILE_SIZE;
    y2 = (y_pos + y_val + height_min - 1) / TILE_SIZE;

    if (y_val > 0) {
        if (map_data.tile[y2][x1] > 0 || map_data.tile[y2][x2] > 0) {
            y_pos = y2 * TILE_SIZE - height_frame;
            y_val = 0;
            on_ground = true;
        }
    }
    else if (y_val < 0) {
        if (map_data.tile[y1][x1] > 0 || map_data.tile[y1][x2] > 0) {
            y_pos = (y1 + 1) * TILE_SIZE;
            y_val = 0;
        }
    }

    if (x_pos < 0) {
        x_pos = 0;
    }
    else if (x_pos + width_frame > map_data.max_x) {
        x_pos = map_data.max_x - width_frame - 1;
    }
}

    bool inDistance (Player& p_player, SDL_Renderer* screen) {
        float player_x_pos = p_player.get_x_pos();
        float player_y_pos = p_player.get_y_pos();

        float check_distance = x_pos - player_x_pos;
        direction = (check_distance < 0);

        if (fabs(y_pos - player_y_pos) <=10 && fabs(check_distance) <= distance) {
            status = direction ? PISTOL_RIGHT : PISTOL_LEFT;
            return true;
                }
                return false;
        }

    bool is_seen(Player& p_player, Map& map_data) {
        float player_x_pos = p_player.get_x_pos();
        float player_y_pos = p_player.get_y_pos();

        float check_distance = x_pos - player_x_pos;

        int start_x = min((int)(x_pos / TILE_SIZE), (int)(player_x_pos / TILE_SIZE));
        int end_x = max((int)(x_pos / TILE_SIZE), (int)(player_x_pos / TILE_SIZE));

        if (fabs(y_pos - player_y_pos) <= 10 && fabs(check_distance) <= distance) {
            for (int i = start_x; i <= end_x; i++) {
                if (map_data.tile[(int)(player_y_pos / TILE_SIZE)][i] != 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
}


    void DoEnemy (SDL_Renderer* screen, Map& map_data) {
            if (isDead) return;
            bool near = inDistance(*p_player, screen);
            bool seen = is_seen(*p_player, map_data);
            Uint32 now = SDL_GetTicks();
            if (near && seen) {

                if (!detect) {
                detect = true;
                detection = now;
            }
            if (!holding_rifle) status = direction ? PISTOL_RIGHT : PISTOL_LEFT;
                else status = direction ? RIFLE_RIGHT : RIFLE_LEFT;
                cooldown = holding_rifle ? 400 : 600;
            if (now - detection >= delay_first) {
            if (now - shot >= delay_shooting) {
                if (!is_shooting && now - shot >= cooldown) {
                    Bullet* bullet = new Bullet();
                    shot = now;
                if (direction) {
                    bullet->loadBullet("Assets/animations/bullet_right.png", screen);
                    bullet->setHor(10);
                    bullet->setPos(x_pos + 5, y_pos + 2);
                }
                else {
                    bullet->loadBullet("Assets/animations/bullet_left.png",screen);
                    bullet->setHor(-10);
                    bullet->setPos(x_pos - 45, y_pos + 2);
                }
                bullet_list.push_back(bullet);
                    }
                }
            }
            }
            else {
                detect = false;
                status = direction ? STANDING_RIGHT : STANDING_LEFT;
            }
            y_val += GRAVITY_SPEED;
            if (y_val >= MAX_FALL_SPEED) {
                y_val = MAX_FALL_SPEED;
            }

            x_val = 0;
    if (isDamaged) {
        damaged_delay--;
        status = direction ? DAMAGED_LEFT : DAMAGED_RIGHT;

        if (damaged_delay <= 0) {
        isDamaged = false;

        if (isDead) {
            status = direction ? DEAD_LEFT : DEAD_RIGHT;
        } else if (seen) {
            status = direction ? PISTOL_LEFT : PISTOL_RIGHT;
        } else {
            status = direction ? STANDING_LEFT : STANDING_RIGHT;
        }
        }

    return;
    }

            check_to_map(map_data);
            return;
        }

    SDL_Rect getRect() const {
        SDL_Rect r;
        r.x = static_cast<int>(x_pos);
        r.y = static_cast<int>(y_pos);
        r.w = width_frame;
        r.h = height_frame;
        return r;
    }

    void checkHit(vector<Bullet*>& bullets) {
        if (isDead) return;
    for (int i = 0; i < bullets.size(); ++i) {
        if (check_interaction(this->getRect(), bullets[i]->getRect()) && !isDamaged) {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            i--;
            cout << "Enemy hit!" << endl;
            health-=damage;
            isDamaged = true;
            status = direction ? DAMAGED_RIGHT : DAMAGED_LEFT;
            damaged_delay = 60;
        }
        if (health <= 0) {
            status = direction ? DEAD_LEFT : DEAD_RIGHT;
            isDead = true;
        }
        }
    }

    vector<Bullet*>& getBulletlist() {
    return bullet_list;
    }

    void setWeapon() {
        holding_rifle = true;
    }

    void renderHealthBar(SDL_Renderer* renderer) {
    if (health <= 0) return;
    SDL_Rect bg = {(int) x_pos + width_frame/2 - 15, (int)y_pos - 1, 30, 2};
    SDL_Rect fg = {(int) x_pos + width_frame/2 - 15, (int)y_pos - 1, (int)(30 * (health / (float)max_health)), 2};

    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bg);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &fg);
    }

    void get_player_weapon(int type) {
        switch (type) {
    case PISTOL:
        damage = 1;
        break;
    case RIFLE:
        damage = 0.5;
        break;
    case BAZOKA:
        damage = 3;
        break;
        }
    }
    bool is_dead() const { return isDead; }
    bool isCounted() const { return counted; }
    void setCounted(bool value) { counted = value; }

private:
    float x_val = 0;
    float y_val = 0;

    float x_pos;
    float y_pos;

    int width_frame = 0;
    int height_frame = 0;

    int stand_width_frame = 0;
    int stand_height_frame = 0;

    int pistol_width_frame = 0;
    int pistol_height_frame = 0;

    int rifle_width_frame = 0;
    int rifle_height_frame = 0;

    int damaged_width_frame = 0;
    int damaged_height_frame = 0;

    int dead_width_frame = 0;
    int dead_height_frame = 0;

    SDL_Rect frame_clip_standing[3];
    SDL_Rect frame_clip_pistol[1];
    SDL_Rect frame_clip_rifle[1];
    SDL_Rect frame_clip_damaged[1];
    SDL_Rect frame_clip_dead[1];

    Movement input_type;
    int frame = 0;
    int status = 0;

    SDL_Rect rect;
    SDL_Texture* texture = nullptr;

    SDL_Texture* texture_standing_left = nullptr;
    SDL_Texture* texture_standing_right = nullptr;

    SDL_Texture* texture_pistol_left = nullptr;
    SDL_Texture* texture_pistol_right = nullptr;

    SDL_Texture* texture_rifle_left = nullptr;
    SDL_Texture* texture_rifle_right = nullptr;

    SDL_Texture* texture_damaged_left = nullptr;
    SDL_Texture* texture_damaged_right = nullptr;

    SDL_Texture* texture_dead_left = nullptr;
    SDL_Texture* texture_dead_right = nullptr;

    int animation_delay_counter = 0;
    int animation_delay_max = 10;

    bool on_ground = false;
    bool is_shooting = false;
    bool direction = false; // true = right / false = left

    Uint32 shot = 0;
    Uint32 cooldown = 0;

    Uint32 detection = 0;
    Uint32 delay_shooting = 1000;
    Uint32 delay_first = 300;
    bool detect = false;

    float distance = 150;

    vector<Bullet*> bullet_list;

    float health = 3;
    float max_health = 3;
    float damage = 0;

    bool isDamaged = false;
    bool isDead = false;
    bool counted = false;

    bool holding_rifle = false;

    int damaged_delay = 0;
};

#endif // ENEMY_H
