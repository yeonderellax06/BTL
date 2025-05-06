#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <SDL.h>
#include "Player.h"
#include "Platform.h"

class GameScene {
public:
    GameScene(SDL_Renderer* renderer);
    ~GameScene();

    void reset();
    void update(float gravity, float jumpForce);
    void render(SDL_Renderer* renderer);
    void renderBackgroundOnly(SDL_Renderer* renderer);
    Player& getPlayer();
    bool isGameOver() const;
    bool loadTextures(SDL_Renderer* renderer);

    SDL_Texture* normalPlatformTex;
    SDL_Texture* breakablePlatformTex;
    SDL_Texture* moveablePlatformTex;
    SDL_Texture* brokenPlatformTex;


private:
    SDL_Texture* background;
    SDL_Texture* platformTexture;
    Player player;
    PlatformManager platformManager;

};

#endif
