#include "Pause.h"
#include <SDL_image.h>
#include <iostream>

const std::string PAUSE_ICON = "images/pause.png";
const std::string PAUSE_COVER = "images/pause-cover.png";

PauseScene::PauseScene(SDL_Renderer* renderer)
    : paused(false), pauseTexture(nullptr), buttonTexture(nullptr), pauseCoverTexture(nullptr){

    SDL_Surface* iconSurface = IMG_Load(PAUSE_ICON.c_str());
    if (!iconSurface) {
        SDL_Log("Failed to load pause icon: %s", IMG_GetError());
        return;
    }
    pauseTexture = SDL_CreateTextureFromSurface(renderer, iconSurface);
    SDL_FreeSurface(iconSurface);
    pauseIconBox = { SCREEN_WIDTH - 30.0f, 10.0f, 14, 19 };

    SDL_Surface* coverSurface = IMG_Load(PAUSE_COVER.c_str());
    if (!coverSurface) {
        SDL_Log("Failed to load pause cover: %s", IMG_GetError());
        return;
    }
    pauseCoverTexture = SDL_CreateTextureFromSurface(renderer, coverSurface);
    SDL_FreeSurface(coverSurface);


    SDL_Surface* buttonSurface = IMG_Load(BUTTON_IMAGE.c_str());
    if (!buttonSurface) {
        SDL_Log("Failed to load button image: %s", IMG_GetError());
        return;
    }
    buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_FreeSurface(buttonSurface);

    SDL_FRect resumeBox = {
        50,
        350,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
        };
    SDL_FRect playAgainBox = {
        SCREEN_WIDTH - 150,
        350,
        BUTTON_WIDTH,
        BUTTON_HEIGHT};
    SDL_FRect quitBox = {
        (SCREEN_WIDTH - BUTTON_WIDTH) / 2.0f,
        450,
        BUTTON_WIDTH,
        BUTTON_HEIGHT};
    buttons.emplace_back(resumeBox, buttonTexture, BUTTON_CUTS[2], ButtonType::Resume);
    buttons.emplace_back(playAgainBox, buttonTexture, BUTTON_CUTS[4], ButtonType::PlayAgain);
    buttons.emplace_back(quitBox, buttonTexture, BUTTON_CUTS[1], ButtonType::Quit);
}

PauseScene::~PauseScene() {
    if (pauseTexture) SDL_DestroyTexture(pauseTexture);
    if (buttonTexture) SDL_DestroyTexture(buttonTexture);
    if (pauseCoverTexture) SDL_DestroyTexture(pauseCoverTexture);
}

void PauseScene::render(SDL_Renderer* renderer) {
    if (!paused)
        SDL_RenderCopyF(renderer, pauseTexture, nullptr, &pauseIconBox);
    else {
        SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, pauseCoverTexture, nullptr, &fullScreen);

        for (const auto& button : buttons)
            button.draw(renderer);
    }
}

Button* PauseScene::checkClick(int mx, int my) {
    if (!paused) {
        if (mx >= pauseIconBox.x && mx <= pauseIconBox.x + pauseIconBox.w &&
            my >= pauseIconBox.y && my <= pauseIconBox.y + pauseIconBox.h) {
            paused = true;
            return nullptr;
        }
    } else {
        for (auto& button : buttons) {
            if (button.contains(mx, my))
                return &button;
        }
    }
    return nullptr;
}

bool PauseScene::isPaused() const {
    return paused;
}

void PauseScene::setPaused(bool p) {
    paused = p;
}
