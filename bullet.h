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
            return false;
        }
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

        bullet_width_frame = rect.w / 3;
        bullet_height_frame = rect.h;

        for (int i = 0; i < 3; i++) {
            frame_clip_bullet[i].x = i * bullet_width_frame;
            frame_clip_bullet[i].y = 0;
            frame_clip_bullet[i].w = bullet_width_frame;
            frame_clip_bullet[i].h = bullet_height_frame;
        }
        return true;
    }

    void setPos(float x, float y) {
        rect.x = x;
        rect.y = y;
    }

    void setHor(float x_vel) {
        x_val = x_vel;
        is_moving = true;
    }

    void DrawBullet(SDL_Renderer* renderer) {
        if (texture != nullptr) SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    }

    bool OutRange() {
        return rect.x < 0 || rect.x > SCREEN_WIDTH;
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

};

#endif // BULLET_H
