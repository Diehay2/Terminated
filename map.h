#ifndef MAP_H
#define MAP_H

#include "SDL_utils.h"

using namespace std;

#define MAX_MAP_X 40
#define MAX_MAP_Y 20
#define TILE_SIZE 32
#define MAX_TILES 5

struct Map {
    int start_x;
    int start_y;

    int max_x;
    int max_y;

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
    map_data.max_x = 0;
    map_data.max_y = 0;

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
        if (j > map_data.max_x) map_data.max_x = j;
        if (i > map_data.max_y) map_data.max_y = i;
            }
        }
    }

map_data.max_x = (map_data.max_x + 1)* TILE_SIZE;
map_data.max_y = (map_data.max_y + 1)* TILE_SIZE;
map_data.start_x = 0;
map_data.start_y = 0;

file.close();

}

void loadTiles(SDL_Renderer* screen) {
    for (int i = 0; i < MAX_TILES; i++) {
        if (i>0) {
        string filename = "Assets/tileset/main_tileset" + to_string(i-1) + ".png";

        tile_mat[i] = loadImg(filename, screen);

        if (tile_mat[i] == nullptr) {
            logError_Exit("Tiles", SDL_GetError());
        }
        }
    }
}

void DrawMap(SDL_Renderer* renderer) {
    int x_offset = map_data.start_x;
    int y_offset = map_data.start_y;

    for (int i = 0; i < MAX_MAP_Y; ++i) {
        for (int j = 0; j < MAX_MAP_X; ++j) {
            int tile_type = map_data.tile[i][j];

            if (tile_type >= 0 && tile_type < MAX_TILES && tile_mat[tile_type] != nullptr) {
                SDL_Rect dst;
                dst.x = j * TILE_SIZE - x_offset;
                dst.y = i * TILE_SIZE - y_offset;
                dst.w = TILE_SIZE;
                dst.h = TILE_SIZE;

                SDL_RenderCopy(renderer, tile_mat[tile_type], nullptr, &dst);
            }
        }
    }
}

Map& getMap() {
    return map_data;
}

private:
    Map map_data;
    SDL_Texture* tile_mat[MAX_TILES];
};

#endif // MAP_H
