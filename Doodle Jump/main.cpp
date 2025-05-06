#include<iostream>
#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<vector>
#include<ctime>

#include "Player.h"
#include "Platform.h"
#include "GameOver.h"
#include "constants.h"
#include "Menu.h"
#include "Pause.h"
#include "GameScene.h"

using namespace std;

const char* TITLE = "Doodle Jump";

enum class GameState { Menu, Playing, GameOver};


void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window* window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    SDL_Renderer* renderer = createRenderer(window);


    SDL_Texture* menuCover = IMG_LoadTexture(renderer, "images/menu-cover.png");
    SDL_Texture* pauseTexture = IMG_LoadTexture(renderer, "images/pause.png");
    SDL_Texture* pauseCover = IMG_LoadTexture(renderer, "images/pause-cover.png");

    SDL_Rect pauseRect = { SCREEN_WIDTH - 30, 10, 14 , 19 };


    Menu menu(renderer);
    GameScene game(renderer);
    GameOverScreen gameOverScreen(renderer);
    PauseScene pauseScreen(renderer);

    if (!game.loadTextures(renderer)) {
    SDL_Log("Failed to load platform textures");
    return -1;
}

    GameState currentState = GameState::Menu;

    float x = 200, y = 300, dy = 0;
    const float gravity = 0.3f, jumpForce = -10.0f;

    bool running = true;
    SDL_Event e;

    while (running){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) running = false;

            else if (e.type == SDL_MOUSEBUTTONDOWN){
                int x,y;
                SDL_GetMouseState(&x, &y);

                if (currentState == GameState::Playing) {
                    Button* clicked = pauseScreen.checkClick(x, y);

                    if (pauseScreen.isPaused()) {
                        if (clicked) {
                            switch (clicked->getType()) {
                            case ButtonType::Resume:
                                pauseScreen.setPaused(false);
                                break;
                            case ButtonType::PlayAgain:
                                pauseScreen.setPaused(false);
                                currentState = GameState::Playing;
                                game.reset();
                                break;
                            case ButtonType::Quit:
                                running = false;
                                break;
                            default:
                                break;
                            }
                        }
                    continue;  // Nếu đang pause thì bỏ qua xử lý bên dưới
                    }
                    SDL_Point mousePoint{ x, y };
                    if (SDL_PointInRect(&mousePoint, &pauseRect)) {
                        pauseScreen.setPaused(true);
                        continue;
                    }
                }
                if (currentState == GameState::Menu){
                    Button* clicked = menu.checkClick(x,y);
                    if (clicked){
                        switch (clicked->getType()){
                            case ButtonType::Play:
                                currentState = GameState::Playing;
                                game.reset();
                                break;
                            case ButtonType::Quit:
                                running = false;
                                break;
                            default:
                                break;
                        }
                    }
                }
                else if (currentState == GameState::GameOver){
                    Button* clicked = gameOverScreen.checkClick(x,y);
                    if (clicked){
                        switch (clicked->getType()){
                        case ButtonType::PlayAgain:

                            gameOverScreen.setGameOver(false);
                            currentState = GameState::Playing;
                            game.reset();
                            break;
                        case ButtonType::Quit:
                            running = false;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }

    SDL_RenderClear(renderer);

    if (currentState == GameState::Menu){
        SDL_RenderCopy(renderer, menuCover, nullptr, nullptr);
        menu.draw(renderer);
    }
    else if (currentState == GameState::Playing){
        game.render(renderer);
        if (pauseScreen.isPaused()) {
            SDL_RenderCopy(renderer, pauseCover, nullptr, nullptr);
            pauseScreen.render(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            continue; // Không update game nếu đang pause
            }
        else SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);


    const Uint8* keys = SDL_GetKeyboardState(NULL);

    game.getPlayer().handleInput(keys);
    game.update(gravity, jumpForce);

            if (game.isGameOver()) {
                gameOverScreen.setGameOver(true);
                currentState = GameState::GameOver;
            }
        }

        else if (currentState == GameState::GameOver) {
            game.renderBackgroundOnly(renderer);
            gameOverScreen.render(renderer);
        }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    }

    SDL_DestroyTexture(menuCover);
     SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(pauseCover);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

