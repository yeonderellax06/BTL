#ifndef MENU_H
#define MENU_H

#include<vector>
#include<SDL.h>
#include <SDL_image.h>
#include "button.h"

class Menu{
public:
    Menu(SDL_Renderer *renderer);
    ~Menu();

    void draw(SDL_Renderer* renderer);
    //void update(unsigned deltaTime);
    Button* checkClick(int mouseX, int mouseY);

private:
    SDL_Texture* buttonTexture;
    std::vector<Button> buttons;
};



#endif // MENU_H
