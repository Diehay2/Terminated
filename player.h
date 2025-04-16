#ifndef PLAYER_H
#define PLAYER_H

#include "SDL_utils.h"

using namespace std;

struct Movement {
    int left;
    int right;
    int down;
    int jump;
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


    bool load_player(const string& path, SDL_Renderer* screen) {
    texture = loadImg(path,screen);
    if (texture == nullptr) {
        logError_Exit("Load Player",SDL_GetError());
        return false;
    }
    else {
        width_frame = rect.w/3;
        height_frame = rect.h;
        return true;
    }
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
    }
    }

    void ShowObject(SDL_Renderer* des) {
    if (status == WALK_LEFT) {
        loadImg("Assets/animations/walking_left.jpg",des);
    }
    else {
        loadImg("Assets/animations/walking_right.jpg",des);
    }
    if (input_type.left == 1|| input_type.right == 1) {
        frame++;
    }
    else {
        frame = 0;
    }

    rect.x = x_pos;
    rect.y = y_pos;

    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};
    SDL_RenderCopy(des,texture,current_clip,&renderQuad);
    }

    void handleInput(SDL_Event input, SDL_Renderer* screen) {
        if (input.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                status = WALK_RIGHT;
                input_type.right = 1;
                break;
            case SDLK_LEFT:
                status = WALK_LEFT;
                input_type.left = 1;
            }
        }
    }

private:
    float x_val = 0;
    float y_val = 0;

    float x_pos = 0;
    float y_pos = 0;

    int width_frame = 0;
    int height_frame = 0;

    SDL_Rect frame_clip[3];
    Movement input_type;
    int frame = 0;
    int status = 0;

    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
};

#endif // PLAYER_H
