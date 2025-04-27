#ifndef BULLET_H
#define BULLET_H

#include "SDL_utils.h"

class Bullet {
public:
    Bullet() {;}
    ~Bullet() {;}

    void fire() {
        if (is_moving) {
            rect.x += x_val;
            rect.y += y_val;
        }
    }

    bool loadBullet(const string& filename, SDL_Renderer* renderer) {
        texture = loadImg(filename, renderer);
        if (texture == nullptr) {
            logError_Exit("Load bullet", SDL_GetError());
        }
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
        return true;
    }

    void setPos(int x, int y) {
        rect.x = x;
        rect.y = y;
    }

    void setHor(int x_vel) {
        x_val = x_vel;
        is_moving = true;
    }

    /*void DrawBullet(SDL_Renderer* texture) {
        SDL_RenderCopy(renderer, texture, nullptr, nullptr));
    }*/

    bool OutRange() {
        return rect.x < 0 || rect.x > SCREEN_WIDTH;
    }

    void setclip_bullet() {

    }

private:
    int x_val = 0;
    int y_val = 0;
    bool is_moving = false;

    SDL_Rect rect;
    SDL_Texture* texture;

    SDL_Rect frame_clip_bullet[3];

    int bullet_width_frame = 0;
    int bullet_height_frame = 0;

    vector<Bullet*> bullet_list;
};

#endif // BULLET_H
