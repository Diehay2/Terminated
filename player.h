#ifndef PLAYER_H
#define PLAYER_H

#include "SDL_utils.h"
#include "map.h"
#include "bullet.h"

using namespace std;

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 5
#define JUMP_HEIGHT -8

struct Movement {
    int left = 0;
    int right = 0;
    int jump = 0;
};

enum Actions {
    STANDING_RIGHT = 0,
    STANDING_LEFT = 1,
    WALK_RIGHT = 2,
    WALK_LEFT = 3,
    JUMP_RIGHT = 4,
    JUMP_LEFT = 5,
    PISTOL_LEFT = 6,
    PISTOL_RIGHT = 7
};

class Player {
public:
    Player(){
        x_pos = 50;
        y_pos = 600;
        rect.x = 0;
        rect.y = 0;
        rect.w = 0;
        rect.h = 0;
    }
    ~Player(){;}

    bool load_player(SDL_Renderer* screen) {
    texture_standing_left = loadImg("Assets/animations/standing_left.png", screen);
    texture_standing_right = loadImg("Assets/animations/standing_right.png", screen);

    texture_walking_left = loadImg("Assets/animations/walking_left.png", screen);
    texture_walking_right = loadImg("Assets/animations/walking_right.png", screen);

    texture_jump_left = loadImg("Assets/animations/jumping_left.png", screen);
    texture_jump_right = loadImg("Assets/animations/jumping_right.png", screen);

    texture_pistol_left = loadImg("Assets/animations/pistol_left.png", screen);
    texture_pistol_right = loadImg("Assets/animations/pistol_right.png", screen);

    if (texture_standing_left == nullptr || texture_standing_right == nullptr
        || texture_walking_left == nullptr || texture_walking_right == nullptr
        || texture_jump_left == nullptr || texture_jump_right == nullptr
        || texture_pistol_left == nullptr || texture_pistol_right == nullptr) {
        logError_Exit("Load Player", SDL_GetError());
        return false;
    }

    SDL_QueryTexture(texture_standing_right, nullptr, nullptr, &rect.w, &rect.h);
    stand_width_frame = rect.w / 3;
    stand_height_frame = rect.h;

    SDL_QueryTexture(texture_walking_right, nullptr, nullptr, &rect.w, &rect.h);
    walk_width_frame = rect.w / 4;
    walk_height_frame = rect.h;

    SDL_QueryTexture(texture_jump_right, nullptr, nullptr, &rect.w, &rect.h);
    jump_width_frame = rect.w / 2;
    jump_height_frame = rect.h;

    SDL_QueryTexture(texture_pistol_right, nullptr, nullptr, &rect.w, &rect.h);
    pistol_width_frame = rect.w;
    pistol_height_frame = rect.h;
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

    void setclip_walking() {
    if (walk_width_frame > 0 && walk_height_frame > 0) {
        for (int i = 0; i < 4; i++) {
            frame_clip_walking[i].x = i * walk_width_frame;
            frame_clip_walking[i].y = 0;
            frame_clip_walking[i].w = walk_width_frame;
            frame_clip_walking[i].h = walk_height_frame;
            }
        }
    }

    void setclip_jumping() {
    if (jump_width_frame > 0 && jump_height_frame > 0) {
        for (int i = 0; i < 2; i++) {
              frame_clip_jump[i].x = i * jump_width_frame;
              frame_clip_jump[i].y = 0;
              frame_clip_jump[i].w = jump_width_frame;
              frame_clip_jump[i].h = jump_height_frame;
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

    void ShowObject(SDL_Renderer* des, Map& map_data) {
    SDL_Rect* current_clip;
    SDL_Texture* current_texture;
    if (status == WALK_LEFT || status == WALK_RIGHT) {
        if (input_type.left == 1 || input_type.right == 1) {
        animation_delay_counter++;
        if (animation_delay_counter >= animation_delay_max) {
            frame++;
            if (frame >= 4) frame = 0;
            animation_delay_counter = 0;
        }
    }

        else {
        frame = 0;
        animation_delay_counter = 0;
    }
    current_clip = &frame_clip_walking[frame];
    current_texture = (status == WALK_LEFT) ? texture_walking_left : texture_walking_right;
    width_frame = walk_width_frame;
    height_frame = walk_height_frame;
    }

    else if (status == JUMP_LEFT || status == JUMP_RIGHT) {
    animation_delay_counter++;
    if (animation_delay_counter >= animation_delay_max) {
        frame++;
        if (frame >= 2) frame = 0;
        animation_delay_counter = 0;
    }
    current_clip = &frame_clip_jump[frame];
    current_texture = (status == JUMP_LEFT) ? texture_jump_left : texture_jump_right;
    width_frame = jump_width_frame;
    height_frame = jump_height_frame;
    }


    else if (status == PISTOL_LEFT || status == PISTOL_RIGHT) {
    frame = 0;
    is_shooting = false;
    current_clip = &frame_clip_pistol[frame];
    current_texture = (status == PISTOL_LEFT || status == STANDING_LEFT)
                    ? texture_pistol_left : texture_pistol_right;
    width_frame = pistol_width_frame;
    height_frame = pistol_height_frame;
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

    SDL_Rect renderQuad = {static_cast<int>(x_pos), static_cast<int>(y_pos + 1), width_frame, height_frame};
    SDL_RenderCopy(des, current_texture, current_clip, &renderQuad);

    for (int i = 0; i < bullet_list.size(); ++i) {
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


    void handleInput(SDL_Event input, SDL_Renderer* screen) {
        if (input.type == SDL_KEYDOWN) {
            switch (input.key.keysym.sym) {
            case SDLK_RIGHT:
                status = WALK_RIGHT;
                input_type.right = 1;
                input_type.left = 0;
                direction = true;
                break;

            case SDLK_LEFT:
                status = WALK_LEFT;
                input_type.left = 1;
                input_type.right = 0;
                direction = false;
                break;

            case SDLK_UP:
                if (on_ground) {
                    input_type.jump = 1;
                    status = (input_type.left == 1) ? JUMP_LEFT : JUMP_RIGHT;
                }
                break;
            case SDLK_c:
                if (on_ground && !is_shooting) {
                is_shooting = true;
                status = direction ? PISTOL_RIGHT : PISTOL_LEFT;
                frame = 0;
                input_type.left = 0;
                input_type.right = 0;
                Uint32 now = SDL_GetTicks();
                if (is_shooting && now - shot >= cooldown) {
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
                break;
            }
        }
        else if (input.type == SDL_KEYUP) {
            switch (input.key.keysym.sym) {
            case SDLK_RIGHT:
                input_type.right = 0;
                if (!is_shooting) status = STANDING_RIGHT;
                break;
            case SDLK_LEFT:
                input_type.left = 0;
                if (!is_shooting) status = STANDING_LEFT;
                break;
            case SDLK_c:
                is_shooting = false;
                status = direction ? STANDING_RIGHT : STANDING_LEFT;
                break;
        }
    }
    }

    void DoPlayer (Map& map_data) {
        if (status == PISTOL_LEFT || status == PISTOL_RIGHT) {
            y_val += GRAVITY_SPEED;
            if (y_val >= MAX_FALL_SPEED) {
                y_val = MAX_FALL_SPEED;
            }

            x_val = 0;

            check_to_map(map_data);
            return;
        }

        x_val = 0;
        y_val += GRAVITY_SPEED;

        if (y_val >= MAX_FALL_SPEED) {
            y_val = MAX_FALL_SPEED;
        }

        if (input_type.left == 1) {
            x_val -= PLAYER_SPEED;
        }
        else if (input_type.right == 1) {
            x_val +=PLAYER_SPEED;
        }
        if (input_type.jump == 1 && on_ground) {
            y_val = JUMP_HEIGHT;
            on_ground = false;
            if (input_type.left == 1) {
                status = JUMP_LEFT;
            }
            else if (input_type.right == 1) {
                status = JUMP_RIGHT;
            }
            else {
                status = direction ? JUMP_RIGHT : JUMP_LEFT;
            }
        }

        input_type.jump = 0;
        check_to_map(map_data);

        if (on_ground && (status == JUMP_LEFT || status == JUMP_RIGHT)) {
            status = (direction) ? STANDING_RIGHT : STANDING_LEFT;
            frame = 0;
        }
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

private:
    float x_val = 0;
    float y_val = 0;

    float x_pos = 0;
    float y_pos = 0;

    int width_frame = 0;
    int height_frame = 0;

    int stand_width_frame = 0;
    int stand_height_frame = 0;

    int walk_width_frame = 0;
    int walk_height_frame = 0;

    int jump_width_frame = 0;
    int jump_height_frame = 0;

    int pistol_width_frame = 0;
    int pistol_height_frame = 0;

    SDL_Rect frame_clip_jump[2];
    SDL_Rect frame_clip_standing[3];
    SDL_Rect frame_clip_walking[4];
    SDL_Rect frame_clip_pistol[1];

    Movement input_type;
    int frame = 0;
    int status = 0;

    SDL_Rect rect;
    SDL_Texture* texture = nullptr;

    SDL_Texture* texture_standing_left = nullptr;
    SDL_Texture* texture_standing_right = nullptr;

    SDL_Texture* texture_walking_left = nullptr;
    SDL_Texture* texture_walking_right = nullptr;

    SDL_Texture* texture_jump_left = nullptr;
    SDL_Texture* texture_jump_right = nullptr;

    SDL_Texture* texture_pistol_left = nullptr;
    SDL_Texture* texture_pistol_right = nullptr;

    int animation_delay_counter = 0;
    int animation_delay_max = 10;

    bool on_ground = false;
    bool is_shooting = false;
    bool direction = false;

    Uint32 shot = 0;
    Uint32 cooldown = 500;

    vector<Bullet*> bullet_list;
};

#endif // PLAYER_H
