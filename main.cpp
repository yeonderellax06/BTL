#include<iostream>
#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<vector>
#include<ctime>

#include "Player.h"
#include "GameOver.h"

using namespace std;

const int SCREEN_WIDTH = 400;
const char* TITLE = "Doodle Jump";
const int PLAYER_WIDTH = 50, PLAYER_HEIGHT = 50;
const int PLATFORM_WIDTH = 68, PLATFORM_HEIGHT = 14;

struct Platform {
    int x, y;
};

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

    SDL_Texture* doodler = IMG_LoadTexture(renderer, "images/doodle.png");
    SDL_Texture* platformTex = IMG_LoadTexture(renderer, "images/platform.png");
    SDL_Texture* bg = IMG_LoadTexture(renderer, "images/background.png");

if (!doodler) std::cout << "Failed to load doodler: " << IMG_GetError() << std::endl;
if (!platformTex) std::cout << "Failed to load platform: " << IMG_GetError() << std::endl;
if (!bg) std::cout << "Failed to load background: " << IMG_GetError() << std::endl;


    Player player(renderer);
    GameOverScreen gameOverScreen(renderer);

    float x = 200, y = 300, dy = 0;
    const float gravity = 0.3f, jumpForce = -10;

    vector<Platform> platforms(10);
    srand(time(0));
    for (int i = 0; i < platforms.size(); ++i) {
        platforms[i].x = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
        platforms[i].y = i * (SCREEN_HEIGHT / platforms.size());
    }

    bool running = true;
    SDL_Event e;
    while (running){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) running = false;
        }
        if (player.isGameOver()) {
            gameOverScreen.setGameOver(true);
        }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    player.handleInput(keys);
    player.update(gravity, jumpForce);

    SDL_Rect playerRect = player.getRect();

    bool onPlatform = false;
        for (auto& p : platforms) {
            if (playerRect.x + playerRect.w > p.x &&
                playerRect.x < p.x + PLATFORM_WIDTH &&
                playerRect.y + playerRect.h > p.y &&
                playerRect.y + playerRect.h < p.y + PLATFORM_HEIGHT &&
                player.getY() + playerRect.h < SCREEN_HEIGHT)
            {
                onPlatform = true;
                break;
            }
        }
    if (onPlatform && !player.isOnGround()) {
            player.landOnPlatform();
            player.jump(jumpForce); // chỉ nhảy khi tiếp đất

        }

    if (player.getY() < 200) {
            player.setY(200);
            for (auto& p : platforms) {
                p.y -= playerRect.y - 200;
                if (p.y > SCREEN_HEIGHT) {
                    p.y = 0;
                    p.x = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
                }
            }
        }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg, nullptr, nullptr);

    for (auto&p : platforms){
        SDL_Rect platRect = {p.x, p.y, PLATFORM_WIDTH, PLATFORM_HEIGHT};
        SDL_RenderCopy(renderer, platformTex, nullptr, &platRect);
        }
    player.render(renderer);

    if (gameOverScreen.isGameOver()){
        gameOverScreen.render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(3000);
        break;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    }

    SDL_DestroyTexture(doodler);
    SDL_DestroyTexture(platformTex);
    SDL_DestroyTexture(bg);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
