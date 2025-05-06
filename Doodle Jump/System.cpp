#include "System.h"
#include <SDL_image.h>

System::System(SDL_Renderer* renderer)
    : currentState(GameState::Menu),
      menu(renderer), pauseScreen(renderer),
      game(renderer), gameOverScreen(renderer)
{
    menuCover = IMG_LoadTexture(renderer, "images/menu-cover.png");
    pauseTexture = IMG_LoadTexture(renderer, "images/pause.png");
    pauseCover = IMG_LoadTexture(renderer, "images/pause-cover.png");
    pauseRect = { SCREEN_WIDTH - 30, 10, 14, 19 };

    game.loadTextures(renderer);
}

System::~System() {
    SDL_DestroyTexture(menuCover);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(pauseCover);
}

void System::handleEvents(SDL_Event& e, bool& running) {
    if (e.type == SDL_QUIT) {
        running = false;
        return;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (currentState == GameState::Playing) {
            if (pauseScreen.isPaused()) {
                if (Button* clicked = pauseScreen.checkClick(x, y)) {
                    switch (clicked->getType()) {
                        case ButtonType::Resume:
                            pauseScreen.setPaused(false);
                            break;
                        case ButtonType::PlayAgain:
                            pauseScreen.setPaused(false);
                            game.reset();
                            break;
                        case ButtonType::Quit:
                            running = false;
                            break;
                        default: break;
                    }
                }
                return;
            }
            SDL_Point point{ x, y };
            if (SDL_PointInRect(&point, &pauseRect)) {
                pauseScreen.setPaused(true);
                return;
            }
        }

        if (currentState == GameState::Menu) {
            if (Button* clicked = menu.checkClick(x, y)) {
                switch (clicked->getType()) {
                    case ButtonType::Play:
                        currentState = GameState::Playing;
                        game.reset();
                        break;
                    case ButtonType::Quit:
                        running = false;
                        break;
                    default: break;
                }
            }
        }

        else if (currentState == GameState::GameOver) {
            if (Button* clicked = gameOverScreen.checkClick(x, y)) {
                switch (clicked->getType()) {
                    case ButtonType::PlayAgain:
                        gameOverScreen.setGameOver(false);
                        currentState = GameState::Playing;
                        game.reset();
                        break;
                    case ButtonType::Quit:
                        running = false;
                        break;
                    default: break;
                }
            }
        }
    }
}

void System::update(float gravity, float jumpForce) {
    if (currentState == GameState::Playing && !pauseScreen.isPaused()) {
        game.getPlayer().handleInput(SDL_GetKeyboardState(nullptr));
        game.update(gravity, jumpForce);

        if (game.isGameOver()) {
            gameOverScreen.setGameOver(true);
            currentState = GameState::GameOver;
        }
    }
}

void System::render(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
    if (currentState == GameState::Menu) {
        SDL_RenderCopy(renderer, menuCover, nullptr, nullptr);
        menu.draw(renderer);
    }
    else if (currentState == GameState::Playing) {
        game.render(renderer);
        if (pauseScreen.isPaused()) {
            SDL_RenderCopy(renderer, pauseCover, nullptr, nullptr);
            pauseScreen.render(renderer);
        } else {
            SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);
        }
    }
    else if (currentState == GameState::GameOver) {
        game.renderBackgroundOnly(renderer);
        gameOverScreen.render(renderer);
    }
    SDL_RenderPresent(renderer);
}

GameState System::getCurrentState() const {
    return currentState;
}

void System::setState(GameState state) {
    currentState = state;
}

bool System::isPaused() const {
    return pauseScreen.isPaused();
}

