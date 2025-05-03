#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL.h>
#include <string>
#include <vector>

enum class ButtonType {PlayAgain, Menu};

class Button {
public:
    Button (const SDL_FRect &box, SDL_Texture* texture, const SDL_Rect& spriteCut, ButtonType type);
    void draw(SDL_Renderer *renderer);
    bool contains(int x, int y) const;
    ButtonType getType() const;

private:
    SDL_FRect box_;
    SDL_Texture* texture_;
    SDL_Rect spriteCut_;
    ButtonType type_;
};

class GameOverScreen {
public:
    GameOverScreen(SDL_Renderer* renderer);
    ~GameOverScreen();

    void render(SDL_Renderer* render);
    bool isGameOver() const;
    void setGameOver(bool over);
    Button* checkClick (int mx, int my);
private:
    bool gameOver;
    SDL_Texture* gameOverTexture;
    SDL_Rect destRect;
};

#endif // GAMEOVER_H

