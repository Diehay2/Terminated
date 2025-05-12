#ifndef BULLET_H
#define BULLET_H

#include "SDL_utils.h"

class Bullet;

extern vector<Bullet*> enemy_bullet_list;
extern vector<Bullet*> player_bullet_list;

class Bullet {
public:
    Bullet() : texture(nullptr) {}
    ~Bullet() {}

    void fire() {
        if (is_moving) {
            rect.x += static_cast<int>(x_val);
            rect.y += static_cast<int>(y_val);
            distance += static_cast<int>(abs(x_val));
            cout << "bullet x: " << rect.x << " frame: " << current_frame << std::endl;
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

        if (bullet_height_frame > 0 && bullet_width_frame > 0) {
            for (int i = 0; i < 3; i++) {
                frame_clip_bullet[i].x = i * bullet_width_frame;
                frame_clip_bullet[i].y = 0;
                frame_clip_bullet[i].w = bullet_width_frame;
                frame_clip_bullet[i].h = bullet_height_frame;
            }
        }
        return true;
    }

    void setPos(int x, int y) {
        rect.x = x;
        rect.y = y;
        current_frame = 0;
    }

    void setHor(float x_vel) {
        x_val = x_vel;
        is_moving = true;
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr) {
            SDL_RenderCopy(renderer, texture, &frame_clip_bullet[current_frame], &rect);
        }
    }

    bool OutRange() {
        return rect.x < 0 || rect.x > SCREEN_WIDTH || distance >= max_range;
    }

    void updateFrame() {
        frame_delay ++;
        if (frame_delay >= frame_delay_max) {
        current_frame = (current_frame + 1) % 3;
        frame_delay = 0;
        }
    }

    SDL_Rect getRect() const {
        return rect;
    }

private:
    float x_val = 0;
    float y_val = 0;
    bool is_moving = false;

    SDL_Rect rect;
    SDL_Texture* texture;

    SDL_Rect frame_clip_bullet[3];
    int current_frame = 0;
    int frame_delay = 0;
    const int frame_delay_max = 15;

    int bullet_width_frame = 0;
    int bullet_height_frame = 0;

    int max_range = 100;
    int distance = 0;
};

#endif // BULLET_H

