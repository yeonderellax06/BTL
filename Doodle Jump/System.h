#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h>
#include "Menu.h"
#include "Pause.h"
#include "GameScene.h"
#include "GameOver.h"


enum class GameState { Menu, Playing, GameOver };

class System {
public:
    System(SDL_Renderer* renderer);
    ~System();

    void handleEvents(SDL_Event& e, bool& running);
    void update(float gravity, float jumpForce);
    void render(SDL_Renderer* renderer);

    GameState getCurrentState() const;
    void setState(GameState state);
    bool isPaused() const;

private:
    GameState currentState;
    Menu menu;
    PauseScene pauseScreen;
    GameScene game;
    GameOverScreen gameOverScreen;

    SDL_Texture* menuCover;
    SDL_Texture* pauseTexture;
    SDL_Texture* pauseCover;
    SDL_Rect pauseRect;
};

#endif

