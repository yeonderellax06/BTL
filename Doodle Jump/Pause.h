#ifndef PAUSE_H
#define PAUSE_H

#include <SDL.h>
#include <vector>
#include "Button.h"
#include "constants.h"

class PauseScene {
public:
    PauseScene(SDL_Renderer* renderer);
    ~PauseScene();

    void render(SDL_Renderer* renderer);
    Button* checkClick(int mx, int my);
    bool isPaused() const;
    void setPaused(bool p);

private:
    SDL_Texture* pauseTexture;
    SDL_Texture* pauseCoverTexture;
    SDL_FRect pauseIconBox;
    SDL_Texture* buttonTexture;
    std::vector<Button> buttons;
    bool paused;
};

#endif
