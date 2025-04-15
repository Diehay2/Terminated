#ifndef PLAYER_H
#define PLAYER_H

#include "SDL_utils.h"

using namespace std;

const int Colour_R = 167;
const int Colour_G = 175;
const int Colour_B = 180;

struct Movement {

    int left_;
    int right_;
    int down_;
    int jump_;

};

class Player {
public:
    Player(){;}
    ~Player(){;}

private:
    float x_val = 0;

    float x_pos = 0;

    int width_frame = 0;
    int height_frame = 0;

    SDL_Rect frame_clip_[3];
    Movement input_type_;
    int frame_ = 0;
    int status_ = 0;

};

#endif // PLAYER_H
