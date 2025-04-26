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

    SDL_Texture* platformTex = IMG_LoadTexture(renderer, "images/platform.png");
    SDL_Texture* bg = IMG_LoadTexture(renderer, "images/background.png");
    SDL_Texture* menuCover = IMG_LoadTexture(renderer, "images/menu-cover.png");
/*if (!doodler) std::cout << "Failed to load doodler: " << IMG_GetError() << std::endl;
if (!platformTex) std::cout << "Failed to load platform: " << IMG_GetError() << std::endl;
if (!bg) std::cout << "Failed to load background: " << IMG_GetError() << std::endl;
*/

    Menu menu(renderer);
    Player player(renderer);
    PlatformManager platformManager;
    GameOverScreen gameOverScreen(renderer);

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
                if (currentState == GameState::Menu){
                    Button* clicked = menu.checkClick(x,y);
                    if (clicked){
                        switch (clicked->getType()){
                            case ButtonType::Play:
                                running = true;
                                currentState = GameState::Playing;
                                player.reset();
                                platformManager.reset();
                                break;
                            case ButtonType::Quit:
                                running = false;
                                break;
                            case ButtonType::Resume:
                                // tiep tuc choi
                                break;
                            case ButtonType::Menu:
                                currentState = GameState::Menu;
                                break;
                            case ButtonType::PlayAgain:
                                //choi lai tu dau
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
                            currentState = GameState::Playing;
                            player.reset();
                            platformManager.reset();
                            gameOverScreen.setGameOver(false);
                            break;
                        case ButtonType::Menu:
                            currentState = GameState::Menu;
                            gameOverScreen.setGameOver(false);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }

    SDL_RenderClear(renderer);
    //SDL_RenderCopy(renderer, bg, nullptr, nullptr);

    if (currentState == GameState::Menu){
        SDL_RenderCopy(renderer, menuCover, nullptr, nullptr);
        menu.draw(renderer);
    }
    else if (currentState == GameState::Playing){
        SDL_RenderCopy(renderer, bg, nullptr, nullptr);


    const Uint8* keys = SDL_GetKeyboardState(NULL);
    player.handleInput(keys);
    player.update(gravity, jumpForce);

    for (auto& platform : platformManager.getPlatforms()) {
            if (player.checkCollision(platform.getRect())){
                player.landOnPlatform();
                player.setY(platform.y - player.getRect().h);
                player.jump(jumpForce);
                break;
            }
        }

    if (player.getY() < 200) {
            float dy = 200 - player.getY();
            player.setY(200);
            platformManager.update(dy);
            }


    platformManager.render(renderer, platformTex);
    player.render(renderer);

     if (player.isGameOver()) {
            gameOverScreen.setGameOver(true);
            currentState = GameState::GameOver;
        }
    }

    else if (currentState == GameState::GameOver){
        SDL_RenderCopy(renderer, bg, nullptr, nullptr);
        gameOverScreen.render(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    }

    SDL_DestroyTexture(platformTex);
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(menuCover);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

