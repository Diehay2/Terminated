#ifndef OBJECT_H
#define OBJECT_H

#include "SDL_utils.h"

class Object {
public:
    Object(): texture(nullptr){;}
    ~Object() {
        free();
    }

    bool loadObject(const string filename, SDL_Renderer* screen) {
        texture = loadImg(filename, screen);

        if (texture == nullptr) {
            logError_Exit("Load object", SDL_GetError());
            return false;
        }
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
        return true;
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr) {
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
        }
    }

    void setPos(float x, float y) {
        x_pos = x;
        y_pos = y;
        rect.x = static_cast<int>(x);
        rect.y = static_cast<int>(y);
    }

    void free() {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

    float get_x_pos () {return x_pos;}
    float get_y_pos () {return y_pos;}

private:
    float x_pos = 0;
    float y_pos = 0;

    SDL_Rect rect{};
    SDL_Texture* texture;
};
#endif // OBJECT_H
