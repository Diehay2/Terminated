#ifndef PLAYER_H
#define PLAYER_H

#include "SDL_utils.h"
#include "map.h"

using namespace std;

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 5

struct Movement {
    int left = 0;
    int right = 0;
    int down = 0;
    int jump = 0;
};

enum Walk {
    WALK_RIGHT = 0,
    WALK_LEFT = 1
};

class Player {
public:
    Player(){
        rect.x = 0;
        rect.y = 0;
        rect.w = 0;
        rect.h = 0;
    }
    ~Player(){;}

    bool load_player(SDL_Renderer* screen) {
    texture_left = loadImg("Assets/animations/walking_left.png", screen);
    texture_right = loadImg("Assets/animations/walking_right.png", screen);

    if (texture_left == nullptr || texture_right == nullptr) {
        logError_Exit("Load Player", SDL_GetError());
        return false;
    }

    SDL_QueryTexture(texture_right, nullptr, nullptr, &rect.w, &rect.h);
    width_frame = rect.w / 4;
    height_frame = rect.h;
    return true;
}

    void setclip() {
    if (width_frame > 0 && height_frame > 0) {
        frame_clip[0].x = 0;
        frame_clip[0].y = 0;
        frame_clip[0].w = width_frame;
        frame_clip[0].h = height_frame;

        frame_clip[1].x = width_frame;
        frame_clip[1].y = 0;
        frame_clip[1].w = width_frame;
        frame_clip[1].h = height_frame;

        frame_clip[2].x = width_frame*2;
        frame_clip[2].y = 0;
        frame_clip[2].w = width_frame;
        frame_clip[2].h = height_frame;

        frame_clip[3].x = width_frame*3;
        frame_clip[3].y = 0;
        frame_clip[3].w = width_frame;
        frame_clip[3].h = height_frame;
    }
    }

    void ShowObject(SDL_Renderer* des) {
    if (input_type.left == 1 || input_type.right == 1) {
        animation_delay_counter++;
        if (animation_delay_counter >= animation_delay_max) {
            frame++;
            if (frame >= 4) frame = 0;
            animation_delay_counter = 0;
        }
    } else {
        frame = 0;
        animation_delay_counter = 0;
    }

    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect renderQuad = {static_cast<int>(x_pos), static_cast<int>(y_pos +3), width_frame, height_frame};

    SDL_Texture* current_texture = (status == WALK_LEFT) ? texture_left : texture_right;
    SDL_RenderCopy(des, current_texture, current_clip, &renderQuad);
    }


    void handleInput(SDL_Event input, SDL_Renderer* screen) {
        if (input.type == SDL_KEYDOWN) {
            switch (input.key.keysym.sym) {
            case SDLK_RIGHT:
                status = WALK_RIGHT;
                input_type.right = 1;
                input_type.left = 0;
                break;
            case SDLK_LEFT:
                status = WALK_LEFT;
                input_type.left = 1;
                input_type.right = 0;
                break;
            }
        }
        else if (input.type == SDL_KEYUP) {
            switch (input.key.keysym.sym) {
            case SDLK_RIGHT:
                input_type.right = 0;
                break;
            case SDLK_LEFT:
                input_type.left = 0;
                break;
            }
        }
    }

    void DoPlayer (Map& map_data) {
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
        check_to_map(map_data);
    }

void check_to_map(Map &map_data) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;
    y1 = (y_pos) / TILE_SIZE;
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

    int height_min = min(height_frame,TILE_SIZE);
    x1 = (x_pos) / TILE_SIZE;
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

    x_pos += x_val;
    y_pos += y_val;

    if (x_pos < 0) {
        x_pos = 0;
    }
    else if (x_pos + width_frame > map_data.max_x) {
        x_pos = map_data.max_x - width_frame - 1;
    }
}


private:
    float x_val = 0;
    float y_val = 0;

    float x_pos = 0;
    float y_pos = 0;

    int width_frame = 0;
    int height_frame = 0;

    SDL_Rect frame_clip[4];
    Movement input_type;
    int frame = 0;
    int status = 0;

    SDL_Rect rect;
    SDL_Texture* texture = nullptr;

    SDL_Texture* texture_left = nullptr;
    SDL_Texture* texture_right = nullptr;

    int animation_delay_counter = 0;
    int animation_delay_max = 10;

    bool on_ground = false;
};

#endif // PLAYER_H
