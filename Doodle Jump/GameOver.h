#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <vector>

#include "Button.h"

class GameOverScreen {
public:
    GameOverScreen(SDL_Renderer* renderer);
    ~GameOverScreen();

    void render(SDL_Renderer* renderer);
    bool isGameOver() const;
    void setGameOver(bool over);
    void reset();
    Button* checkClick (int mx, int my);

    Mix_Chunk* gameOverSound;
    bool soundPlayed;
private:
    bool gameOver;
    SDL_Texture* gameOverTexture;
    SDL_Rect destRect;

    std::vector<Button> buttons;
};

#endif // GAMEOVER_H

