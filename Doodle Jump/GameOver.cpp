#include "GameOver.h"
#include "constants.h"
#include<SDL_image.h>
#include<iostream>

const std::string GAMEOVER_IMAGE = "images/gameover-cover.png";

GameOverScreen::GameOverScreen(SDL_Renderer *renderer):
    gameOver(false), gameOverTexture(nullptr){

    SDL_Surface* surface = IMG_Load(GAMEOVER_IMAGE.c_str());
    if (!surface){
        SDL_Log("failed to render text surface %s", IMG_GetError());
        return;
    }
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    destRect = {0 , 0, 400 , 640};
    SDL_Surface* buttonSurface = IMG_Load(BUTTON_IMAGE.c_str());
    if (!buttonSurface){
        SDL_Log("Failed to load button image: %s", IMG_GetError());
        return;
    }

    SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_FreeSurface(buttonSurface);

    SDL_FRect playAgainBox = {
        (SCREEN_WIDTH - BUTTON_WIDTH * 2) / 2.0f,
        350,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
        };
    buttons.emplace_back(playAgainBox, buttonTexture, BUTTON_CUTS[4], ButtonType::PlayAgain);

    SDL_FRect quitBox = {
        SCREEN_WIDTH / 2.0f,
        450,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
        };
    buttons.emplace_back(quitBox, buttonTexture, BUTTON_CUTS[1], ButtonType::Quit);
}


GameOverScreen::~GameOverScreen(){
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
    if (!buttons.empty() && buttons[0].getTexture()) SDL_DestroyTexture(buttons[0].getTexture());
}
void GameOverScreen::render(SDL_Renderer* renderer){
    if (gameOver && gameOverTexture) {
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, &destRect);
        for (const auto& button : buttons)
            button.draw(renderer);

    }
}

bool GameOverScreen::isGameOver() const {
    return gameOver;}

void GameOverScreen::setGameOver(bool over){
    gameOver = over;
}

Button* GameOverScreen::checkClick(int mx, int my) {
    for (auto& button : buttons) {
        if (button.contains(mx, my))
            return &button;
    }
    return nullptr;
}

