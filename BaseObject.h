#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "SDL_utils.h"

class BaseObject {
public:
    BaseObject() {
        p_object_ = nullptr;
        rect.x = 0;
        rect.y = 0;
        rect.w = 0;
        rect.h = 0;
    }

    bool loadImg(const string& file_path) {
        SDL_Surface* load_surface = IMG_Load(file_path.c_str());
        if (load_surface != nullptr) {
            SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, Colour_R, Colour_G, Colour_B));
            p_object_ = SDL_CreateTextureFromSurface(renderer, load_surface);
            if (p_object_ != nullptr) {
                rect.w = load_surface->w;
                rect.h = load_surface->h;
            }
            SDL_FreeSurface(load_surface);
            return p_object_ != nullptr;
        }
        return false;
    }

    void render() {
        if (p_object_ != nullptr) {
            SDL_RenderCopy(renderer, p_object_, nullptr, &rect);
        }
    }

protected:
    SDL_Texture* p_object_;
    SDL_Rect rect;
};

#endif // BASEOBJECT_H

