#include "Player.h"

Player::Player(SDL_Renderer* renderer) {
    x = 200;
    y = 300;
    dy = 0;

    onGround = false;
    gameOver = false;

    texLeft = IMG_LoadTexture(renderer, "images/doodle_left.png");
    texRight = IMG_LoadTexture(renderer, "images/doodle_right.png");
    currentTex = texRight;

    rect = { (int)x, (int)y, 50, 50 };
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

    if (y > SCREEN_HEIGHT) {
        gameOver = true;
    }

    rect = { (int)x, (int)y, 50, 50 };
}

void Player::jump(float jumpForce) {
    if (onGround) {
        dy = jumpForce;
        onGround = false;
    }
}

void Player::landOnPlatform() {
    dy = 0;
    onGround = true;
}


bool Player::isOnGround() const {
    return onGround;
}

bool Player::isGameOver() const {
    return gameOver;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, currentTex, nullptr, &rect);
}

float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

void Player::setY(float newY) {
    y = newY;
    rect.y = (int)y;
}

void Player::setX(float newX) {
    x = newX;
    rect.x = (int)x;
}

SDL_Rect Player::getRect() const {
    return rect;
}

bool Player::checkCollision(const SDL_Rect &platformRect){
    SDL_Rect playerRect = getRect();

    bool isFalling = dy >0;
    bool horizontalOverlap = playerRect.x + playerRect.w > platformRect.x &&
                             playerRect.x < platformRect.x + platformRect.w;

    bool verticalCollision = playerRect.y + playerRect.h <= platformRect.y + 10 &&
                             playerRect.y + playerRect.h >= platformRect.y - 10;

    return isFalling && horizontalOverlap && verticalCollision;
}

