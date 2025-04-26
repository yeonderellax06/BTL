#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

enum class ButtonType{
    Play,
    Quit,
    Resume,
    Menu,
    PlayAgain,
    None
};

class Button {
public:
    Button (const SDL_FRect &box, SDL_Texture* texture, const SDL_Rect& spriteCut, ButtonType type);
    void draw(SDL_Renderer *renderer);
    bool contains(int x, int y) const;
    ButtonType getType() const;

private:
    SDL_FRect box_;
    SDL_Texture* texture_;
    SDL_Rect spriteCut_;
    ButtonType type_;
};


#endif // BUTTON_H
