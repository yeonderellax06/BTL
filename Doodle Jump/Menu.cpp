#include "Menu.h"
#include<string>


const std::string BUTTON_IMAGE = "images/buttons.png";

const SDL_Rect BUTTON_CUTS[] = {
{0, 0, 140, 50},
{140, 0, 140, 50},
{2 * 140, 0, 140, 50},
{3 * 140, 0, 140, 50},
{4 * 140, 0, 140, 50},
};

const ButtonType BUTTON_TYPES[] = {
    ButtonType::Play,
    ButtonType::Quit,
    ButtonType::Resume,
    ButtonType::Menu,
    ButtonType::PlayAgain
};

Menu::Menu(SDL_Renderer *renderer){
    SDL_Surface* tempSurface = IMG_Load(BUTTON_IMAGE.c_str());
    if (!tempSurface){
        SDL_Log("failed to load button image:%s", SDL_GetError());
    }
    buttonTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    const float scale = 0.6f;
    const int BUTTON_WIDTH = static_cast<int>(140 * scale);
    const int BUTTON_HEIGHT = static_cast<int>(50 * scale);
    const int startX = 50;
    const int startY = 200;
    const int gap = 10;

    for (int i = 0; i<5; i++){
        SDL_FRect box = {startX + i* (BUTTON_WIDTH + gap), startY, BUTTON_WIDTH, BUTTON_HEIGHT};
        buttons.emplace_back(box, buttonTexture, BUTTON_CUTS[i], BUTTON_TYPES[i]);
    }
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
