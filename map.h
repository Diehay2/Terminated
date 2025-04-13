#ifndef MAP_H
#define MAP_H

#include "SDL_utils.h"
#include <fstream>

using namespace std;

#define MAX_MAP_X 40
#define MAX_MAP_Y 15
#define TILE_SIZE 48
#define MAX_TILES 4

struct Map {
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    vector<vector<int>> tile;
    string filename;
};

class TileMap {
public:
    TileMap() {;}
    ~TileMap(){;}
    void loadMap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        logError_Exit("Load Files",SDL_GetError());
        return;
    }

    map_data.filename = filename;
    map_data.max_x_ = 0;
    map_data.max_y_ = 0;

    map_data.tile.resize(MAX_MAP_Y);
    for (int i = 0; i < MAX_MAP_Y; i++) {
        map_data.tile[i].resize(MAX_MAP_X);
    }

    for (int i = 0; i < MAX_MAP_Y; i++) {
        for (int j = 0; j < MAX_MAP_X; j++) {
            int file_type;
            file >> file_type;
            map_data.tile[i][j] = file_type;
        if (file_type > 0) {
        if (j > map_data.max_x_) map_data.max_x_ = j;
        if (i > map_data.max_y_) map_data.max_y_ = i;
            }
        }
    }

map_data.max_x_ = (map_data.max_x_ + 1)* TILE_SIZE;
map_data.max_y_ = (map_data.max_y_ + 1)* TILE_SIZE;
map_data.start_x_ = 0;
map_data.start_y_ = 0;

file.close();

}

void loadTiles(SDL_Renderer* screen) {
    for (int i = 0; i < MAX_TILES; i++) {
        if (i>0) {
        string filename = "Assets/main_tileset" + to_string(i-1) + ".png";

        tile_mat_[i] = loadImg(filename, screen);

        if (tile_mat_[i] == nullptr) {
            logError_Exit("Tiles", SDL_GetError());
        }
        }
    }
}

void DrawMap(SDL_Renderer* renderer) {
    int x_offset = map_data.start_x_;
    int y_offset = map_data.start_y_;

    for (int i = 0; i < MAX_MAP_Y; ++i) {
        for (int j = 0; j < MAX_MAP_X; ++j) {
            int tile_type = map_data.tile[i][j];

            if (tile_type >= 0 && tile_type < MAX_TILES && tile_mat_[tile_type] != nullptr) {
                SDL_Rect dst;
                dst.x = j * TILE_SIZE - x_offset;
                dst.y = i * TILE_SIZE - y_offset;
                dst.w = TILE_SIZE;
                dst.h = TILE_SIZE;

                SDL_RenderCopy(renderer, tile_mat_[tile_type], nullptr, &dst);
            }
        }
    }
}


private:
    Map map_data;
    SDL_Texture* tile_mat_[MAX_TILES];
};

#endif // MAP_H
