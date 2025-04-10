#include "Player.h"

Player::Player(SDL_Renderer* renderer) {
    x = 200;
    y = 300;
    dy = 0;

    texLeft = IMG_LoadTexture(renderer, "images/doodle_left.png");
    texRight = IMG_LoadTexture(renderer, "images/doodle_right.png");
    currentTex = texRight;
}

Player::~Player() {
    SDL_DestroyTexture(texLeft);
    SDL_DestroyTexture(texRight);
}

void Player::handleInput(const Uint8* keys){
    if (keys[SDL_SCANCODE_LEFT]){
        x -=5;
        currentTex = texLeft;
    }
    if (keys[SDL_SCANCODE_RIGHT]){
        x +=5;
        currentTex = texRight;
    }
}

void Player::update(float gravity, float jumpForce){
    dy += gravity;
    y +=dy;
}

void Player::jump(){
    dy = -10;
}

void Player::render(SDL_Renderer* renderer){
    SDL_Rect rect = { (int) x , (int) y, 50, 50};
    SDL_RenderCopy(renderer, currentTex, nullptr, &rect);
}

float Player::getX() const {return x;}
float Player::getY() const {return y;}
void Player::setY(float newY) { y = newY;}
SDL_Rect Player::getRect() const{
    return {(int) x, (int) y, 50 , 50 };
}
