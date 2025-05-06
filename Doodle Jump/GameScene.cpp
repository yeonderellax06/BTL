#include "GameScene.h"
#include <SDL_image.h>

GameScene::GameScene(SDL_Renderer* renderer)
    : player(renderer), platformManager() {
    background = IMG_LoadTexture(renderer, "images/background.png");
    platformTexture = IMG_LoadTexture(renderer, "images/platform.png");
}

GameScene::~GameScene() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(platformTexture);
}

void GameScene::reset() {
    player.reset();
    platformManager.reset();
}

void GameScene::update(float gravity, float jumpForce) {
    player.update(gravity, jumpForce);

    for (auto& platform : platformManager.getPlatforms()) {
        if (player.checkCollision(platform->getRect())) {
            player.landOnPlatform();
            player.setY(platform->getY() - player.getRect().h);
            player.jump(jumpForce);
            break;
        }
    }

    if (player.getY() < 200) {
        float dy = 200 - player.getY();
        player.setY(200);
        platformManager.update(dy);
    }
    else {
    platformManager.update(0);
    }
}

bool GameScene::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = nullptr;

    tempSurface = IMG_Load("images/platform.png");
    if (!tempSurface) {
        SDL_Log("Failed to load platform.png: %s", SDL_GetError());
        return false;
    }
    normalPlatformTex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load("images/breakable-platform.png");
    if (!tempSurface) {
        SDL_Log("Failed to load breakable-platform.png: %s", SDL_GetError());
        return false;
    }
    breakablePlatformTex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load("images/moveable-platform.png");
    if (!tempSurface) {
        SDL_Log("Failed to load moveable-platform.png: %s", SDL_GetError());
        return false;
    }
    moveablePlatformTex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load("images/broken-platform.png");
    if (!tempSurface) {
        SDL_Log("Failed to load broken-platform.png: %s", SDL_GetError());
        return false;
    }
    brokenPlatformTex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return true;
}


void GameScene::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, background, nullptr, nullptr);
    platformManager.render(renderer, normalPlatformTex, breakablePlatformTex, moveablePlatformTex, brokenPlatformTex);

    player.render(renderer);
}

void GameScene::renderBackgroundOnly(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, background, nullptr, nullptr);
}

bool GameScene::isGameOver() const {
    return player.isGameOver();
}
Player& GameScene::getPlayer() {
    return player;
}
