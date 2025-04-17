#include "GameOver.h"
#include<SDL_image.h>
#include<iostream>

GameOverScreen::GameOverScreen(SDL_Renderer *renderer): gameOver(false), gameOverTexture(nullptr){

    SDL_Surface* surface = IMG_Load("images/gameover-cover.png");
    if (!surface){
        SDL_Log("failed to render text surface %s", IMG_GetError());
        return;
    }
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

}

GameOverScreen::~GameOverScreen(){
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
}
void GameOverScreen::render(SDL_Renderer* renderer){
    if (gameOver && gameOverTexture) {
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, nullptr);

    }
}

bool GameOverScreen::isGameOver() const { return gameOver;}

void GameOverScreen::setGameOver(bool over){
    gameOver = over;
}

