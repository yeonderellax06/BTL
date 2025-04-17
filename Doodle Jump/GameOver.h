#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class GameOverScreen {
public:
    GameOverScreen(SDL_Renderer* renderer);
    ~GameOverScreen();

    void render(SDL_Renderer* render);
    bool isGameOver() const;
    void setGameOver(bool over);
private:
    bool gameOver;
    TTF_Font* font;
    SDL_Texture* messageTexture;
    SDL_Rect messageRect;
};

#endif // GAMEOVER_H

