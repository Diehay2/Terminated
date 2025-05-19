#ifndef MENU_H
#define MENU_H

#include "SDL_utils.h"

enum MenuState {
    MAIN_MENU,
    HELP_SCREEN_MENU,
    HELP_SCREEN_PAUSE,
    PAUSE_MENU,
    WIN,
    LOSE
};

enum MenuResult {
    MENU_NONE,
    MENU_PLAY,
    MENU_HELP,
    MENU_EXIT
};


class Menu {
public:
    Menu(SDL_Window* win, SDL_Renderer* rend) : window(win), renderer(rend),
    playTexture(nullptr), helpTexture(nullptr) , muteTexture(nullptr), resumeTexture(nullptr), logoTexture(nullptr), backgroundTexture(nullptr), helpFrameTexture(nullptr),
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
        SDL_DestroyTexture(helpFrameTexture);
    }

    MenuResult run() {
    SDL_Event e;

    while (true) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                return MENU_EXIT;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                if (currentState != MAIN_MENU &&
                    currentState != HELP_SCREEN_MENU &&
                    currentState != HELP_SCREEN_PAUSE &&
                    currentState != PAUSE_MENU &&
                    currentState != WIN &&
                    currentState != LOSE)
                    currentState = PAUSE_MENU;
            }
            else if (!handleEvent(e)) {
                if (currentState == MAIN_MENU)
                    return MENU_PLAY;
                else if (currentState == HELP_SCREEN_MENU)
                    currentState = MAIN_MENU;
                else if (currentState == PAUSE_MENU)
                    return MENU_PLAY;
                else if (currentState == HELP_SCREEN_PAUSE)
                    currentState = PAUSE_MENU;
                else if (currentState == WIN)
                    currentState = MAIN_MENU;
            }
        }

        render();
        SDL_Delay(16);
    }

    return MENU_NONE;
}

    void setState(MenuState state) {
    currentState = state;
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
    SDL_Texture* helpFrameTexture;

    SDL_Rect playButton = {SCREEN_WIDTH/2 - 100, 400, 200, 50};
    SDL_Rect helpButton_menu = {SCREEN_WIDTH/2 - 100, 500, 200, 50};
    SDL_Rect helpButton_pause = {SCREEN_WIDTH/2 - 100, 300, 200, 50};
    SDL_Rect muteButton = {SCREEN_WIDTH - 20 - 10, SCREEN_HEIGHT - 20, 20, 20};
    SDL_Rect resumeButton = {SCREEN_WIDTH/2 - 100, 200, 200, 50};
    SDL_Rect logoRect = {SCREEN_WIDTH/2 - 750/2, 50, 750, 300};
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect helpFrameRect = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 300, 630, 600};

    MenuState currentState;
    bool isMuted;

    void loadResources() {
        playTexture = loadImg("Assets/Logo/menu_bar.png", renderer);
        helpTexture = loadImg("Assets/Logo/menu_bar.png", renderer);
        muteTexture = loadImg("Assets/Logo/Icon_37.png", renderer);
        resumeTexture = loadImg("Assets/Logo/menu_bar.png", renderer);
        logoTexture = loadImg("Assets/Logo/Logo1.png", renderer);
        backgroundTexture = loadImg("Assets/background.png", renderer);
        helpFrameTexture = loadImg("Assets/Logo/frame.png", renderer);
    }

    bool handleEvent(SDL_Event& e) {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int x = e.button.x;
            int y = e.button.y;

            if (currentState == MAIN_MENU) {
                if (inside(x, y, playButton)) {
                    cout << "Play selected\n";
                    return false;
                } else if (inside(x, y, helpButton_menu)) {
                    currentState = HELP_SCREEN_MENU;
                } else if (inside(x, y, muteButton)) {
                    isMuted = !isMuted;
                }
            } else if (currentState == HELP_SCREEN_MENU) {
                currentState = MAIN_MENU;
            } else if (currentState == HELP_SCREEN_PAUSE) {
                currentState = PAUSE_MENU;
            } else if (currentState == PAUSE_MENU) {
                if (inside(x, y, resumeButton)) {
                    return false;
                }
                else if (inside(x, y, helpButton_pause)) {
                    currentState = HELP_SCREEN_PAUSE;
                }
            } else if (currentState == WIN) {
                currentState = MAIN_MENU;
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
            renderText("TERMINATED", SCREEN_WIDTH/2 - 250, 120, font_title);
            SDL_RenderCopy(renderer, playTexture, nullptr, &playButton);
            renderText("Play", SCREEN_WIDTH/2 - 40, 410, font_button);
            SDL_RenderCopy(renderer, helpTexture, nullptr, &helpButton_menu);
            renderText("Help", SCREEN_WIDTH/2 - 40, 510, font_button);
            SDL_RenderCopy(renderer, muteTexture, nullptr, &muteButton);

        } else if (currentState == HELP_SCREEN_MENU) {
            SDL_RenderCopy(renderer, helpFrameTexture, nullptr, &helpFrameRect);
            renderText("Use arrow keys to move", SCREEN_WIDTH/2 - 200, 100, font_button);
            renderText("Press C to shoot", SCREEN_WIDTH/2 - 200, 150, font_button);
            renderText("Pick up weapons to upgrade", SCREEN_WIDTH/2 - 200, 200, font_button);
            renderText("your gun", SCREEN_WIDTH/2 - 200, 250, font_button);
            renderText("Kill all the enemies to win", SCREEN_WIDTH/2 - 200, 300, font_button);
            renderText("the game", SCREEN_WIDTH/2 - 200, 350, font_button);
            renderText("Click anywhere to return", SCREEN_WIDTH/2 - 200, 550, font_button);
        }else if (currentState == HELP_SCREEN_PAUSE) {
            SDL_RenderCopy(renderer, helpFrameTexture, nullptr, &helpFrameRect);
            renderText("Use arrow keys to move", SCREEN_WIDTH/2 - 200, 100, font_button);
            renderText("Press C to shoot", SCREEN_WIDTH/2 - 200, 150, font_button);
            renderText("Pick up weapons to upgrade", SCREEN_WIDTH/2 - 200, 200, font_button);
            renderText("your gun", SCREEN_WIDTH/2 - 200, 250, font_button);
            renderText("Kill all the enemies to win", SCREEN_WIDTH/2 - 200, 300, font_button);
            renderText("the game", SCREEN_WIDTH/2 - 200, 350, font_button);
            renderText("Click anywhere to return", SCREEN_WIDTH/2 - 200, 550, font_button);
        } else if (currentState == PAUSE_MENU) {
            SDL_RenderCopy(renderer, resumeTexture, nullptr, &resumeButton);
            renderText("Resume", SCREEN_WIDTH/2 - 60, 210, font_button);
            SDL_RenderCopy(renderer, helpTexture, nullptr, &helpButton_pause);
            renderText("Help", SCREEN_WIDTH/2 - 40, 310, font_button);
            SDL_RenderCopy(renderer, muteTexture, nullptr, &muteButton);
        } else if (currentState == WIN) {
            SDL_RenderCopy(renderer, helpFrameTexture, nullptr, &helpFrameRect);
            renderText("WIN", SCREEN_WIDTH/2 - 60, 80, font_title);
            renderText("As the whole organisation got" , SCREEN_WIDTH/2 - 250, 190, font_button);
            renderText("wiped out, you finally put an", SCREEN_WIDTH/2 - 250, 220, font_button);
            renderText("end on this long lasting case.", SCREEN_WIDTH/2 - 250, 250, font_button);
            renderText("Leaving the building with mixed", SCREEN_WIDTH/2 - 250, 280, font_button);
            renderText("feelings, wandering around with", SCREEN_WIDTH/2 - 250, 310, font_button);
            renderText("no purpose, you then decided to", SCREEN_WIDTH/2 - 250, 340, font_button);
            renderText("call it a day and lived a", SCREEN_WIDTH/2 - 250, 370, font_button);
            renderText("peaceful life", SCREEN_WIDTH/2 - 250, 400, font_button);
            renderText("Click anywhere to return", SCREEN_WIDTH/2 - 200, 550, font_button);
        }

        SDL_RenderPresent(renderer);
    }

    bool inside(int x, int y, SDL_Rect& rect) {
        return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
    }
};


#endif // MENU_H
