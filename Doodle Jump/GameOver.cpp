#include "GameOver.h"

GameOverScreen::GameOverScreen(SDL_Renderer *renderer): gameOver(false), font(nullptr), messageTexture(nullptr){
    font = TTF_OpenFont("fonts/arial.ttf",48);
    if (!font){
        SDL_Log("failed to load font: %s", TTF_GetError());
        return;
    }
    SDL_Color color = {255, 0 , 0};
    SDL_Surface* surface = TTF_RenderText_Solid(font,"Game Over",color);
    if (!surface){
        SDL_Log("failed to render text surface %s", TTF_GetError());
        return;
    }
    messageTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!messageTexture) SDL_Log("failed to create text texture: %s",SDL_GetError());

    messageRect.x = (400 - surface->w)/2;
    messageRect.y = (600 - surface->h)/2;
    messageRect.w = surface->w;
    messageRect.h = surface->h;

    SDL_FreeSurface(surface);
}

GameOverScreen::~GameOverScreen(){
    if (messageTexture) SDL_DestroyTexture(messageTexture);
    if (font) TTF_CloseFont(font);
}
void GameOverScreen::render(SDL_Renderer* renderer){
    if (gameOver && messageTexture) {
        SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);

    }
}

bool GameOverScreen::isGameOver() const { return gameOver;}

void GameOverScreen::setGameOver(bool over){
    gameOver = over;
}

