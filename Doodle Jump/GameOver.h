#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL.h>
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
    SDL_Texture* gameOverTexture;
    SDL_Rect destRect;
};

#endif // GAMEOVER_H

