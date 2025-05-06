#include "Menu.h"
#include "constants.h"
#include "Button.h"
#include<string>



Menu::Menu(SDL_Renderer *renderer){
    SDL_Surface* tempSurface = IMG_Load(BUTTON_IMAGE.c_str());
    if (!tempSurface){
        SDL_Log("failed to load button image:%s", SDL_GetError());
    }
    buttonTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    SDL_FRect playBox = {
        (SCREEN_WIDTH - BUTTON_WIDTH * 2) / 2.0f,
        150,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
    };
    buttons.emplace_back(playBox, buttonTexture, BUTTON_CUTS[0], ButtonType::Play);

    SDL_FRect quitBox = {
        SCREEN_WIDTH / 2.0f + 60,
        370,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
    };
    buttons.emplace_back(quitBox, buttonTexture, BUTTON_CUTS[1], ButtonType::Quit);

}

Menu::~Menu(){
if (buttonTexture){
    SDL_DestroyTexture(buttonTexture);
}
}

void Menu::draw(SDL_Renderer *renderer){
    for (auto& button : buttons){
        button.draw(renderer);
    }
}

Button* Menu::checkClick(int mouseX, int mouseY){
    for (auto& button : buttons){
        if (button.contains(mouseX, mouseY)){
            return &button;
        }
    }
    return nullptr;
}
