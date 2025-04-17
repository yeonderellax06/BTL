#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include "constants.h"



class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void handleInput(const Uint8* keys);
    void update(float gravity, float jumpForce);
    void render(SDL_Renderer* renderer);

    float getX() const;
    float getY() const;
    void setX(float NewX);
    void setY(float NewY);
    SDL_Rect getRect() const;

    void jump(float jumpForce);
    void landOnPlatform();
    bool isOnGround() const;
    bool isGameOver() const;
    bool checkCollision(const SDL_Rect& platformRect);
private:
    float x, y, dy;
    SDL_Texture* texLeft;
    SDL_Texture* texRight;
    SDL_Texture* currentTex;
    SDL_Rect rect;

    bool onGround;
    bool gameOver;
};

#endif // PLAYER_H

