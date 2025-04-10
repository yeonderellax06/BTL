#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void handleInput(const Uint8* keys);
    void update(float gravity, float jumpForce);
    void render(SDL_Renderer* renderer);

    float getX() const;
    float getY() const;
    void setY(float NewY);
    SDL_Rect getRect() const;

    void jump();
private:
    float x, y, dy;
    SDL_Texture* texLeft;
    SDL_Texture* texRight;
    SDL_Texture* currentTex;
};

#endif // PLAYER_H
