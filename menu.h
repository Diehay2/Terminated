#ifndef MENU_H
#define MENU_H

#include "SDL_utils.h"

enum MenuState {
    MAIN_MENU,
    HELP_SCREEN,
    PAUSE_MENU,
    WIN,
    LOSE
};

class Menu {
public:
    Menu(SDL_Window* win, SDL_Renderer* rend) : window(win), renderer(rend),
    playTexture(nullptr), helpTexture(nullptr) , muteTexture(nullptr), resumeTexture(nullptr), logoTexture(nullptr), backgroundTexture(nullptr),
    currentState(MAIN_MENU), isMuted(false) {
        loadResources();
    }

    ~Menu() {
        SDL_DestroyTexture(playTexture);
        SDL_DestroyTexture(helpTexture);
        SDL_DestroyTexture(muteTexture);
        SDL_DestroyTexture(resumeTexture);
        SDL_DestroyTexture(logoTexture);
        SDL_DestroyTexture(backgroundTexture);
    }

    void run() {
        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT)
                    quit = true;
                else if (!handleEvent(e))
                    quit = false;
            }

            render();
            SDL_Delay(16);
        }
    }


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    SDL_Texture* playTexture;
    SDL_Texture* helpTexture;
    SDL_Texture* muteTexture;
    SDL_Texture* resumeTexture;
    SDL_Texture* logoTexture;
    SDL_Texture* backgroundTexture;

    SDL_Rect playButton = {SCREEN_WIDTH/2 - 100, 400, 200, 50};
    SDL_Rect helpButton = {SCREEN_WIDTH/2 - 100, 500, 200, 50};
    SDL_Rect muteButton = {SCREEN_WIDTH - 20 - 10, SCREEN_HEIGHT - 20, 20, 20};
    SDL_Rect resumeButton = {SCREEN_WIDTH/2 - 100, 200, 200, 50};
    SDL_Rect logoRect = {SCREEN_WIDTH/2 - 750/2, 50, 750, 300};
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    MenuState currentState;
    bool isMuted;

    void loadResources() {
        playTexture = loadImg("Assets/Logo/menu_bar.png", renderer);
        helpTexture = loadImg("Assets/Logo/menu_bar.png", renderer);
        muteTexture = loadImg("Assets/Logo/Icon_37.png", renderer);
        resumeTexture = loadImg("Assets/Logo/menu_bar.png", renderer);
        logoTexture = loadImg("Assets/Logo/Logo1.png", renderer);
        backgroundTexture = loadImg("Assets/background.png", renderer);
    }

    bool handleEvent(SDL_Event& e) {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int x = e.button.x;
            int y = e.button.y;

            if (currentState == MAIN_MENU) {
                if (inside(x, y, playButton)) {
                    cout << "Play selected\n";
                    return false;
                } else if (inside(x, y, helpButton)) {
                    currentState = HELP_SCREEN;
                } else if (inside(x, y, muteButton)) {
                    isMuted = !isMuted;
                }
            } else if (currentState == HELP_SCREEN) {
                currentState = MAIN_MENU;
            } else if (currentState == PAUSE_MENU) {
                if (inside(x, y, resumeButton)) {
                    return false;
                }
            }
        }

        return true;
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);
        if (currentState == MAIN_MENU) {
            SDL_RenderCopy(renderer, logoTexture, nullptr, &logoRect);
            renderText("NOBODIES", SCREEN_WIDTH/2, 200, font_title);
            SDL_RenderCopy(renderer, playTexture, nullptr, &playButton);
            renderText("Play", SCREEN_WIDTH/2 - 40, 410, font_button);
            SDL_RenderCopy(renderer, helpTexture, nullptr, &helpButton);
            renderText("Help", SCREEN_WIDTH/2 - 40, 510, font_button);
            SDL_RenderCopy(renderer, muteTexture, nullptr, &muteButton);

        } else if (currentState == HELP_SCREEN) {
            renderText("Use arrow keys to move", 100, 150, font_button);
            renderText("Press C to shoot", 100, 200, font_button);
            renderText("Click anywhere to return", 100, 300, font_button);
        } else if (currentState == PAUSE_MENU) {
            SDL_RenderCopy(renderer, resumeTexture, NULL, &resumeButton);
        }

        SDL_RenderPresent(renderer);
    }

    bool inside(int x, int y, SDL_Rect& rect) {
        return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
    }
};


#endif // MENU_H
