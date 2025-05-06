#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <string>

const std::string BUTTON_IMAGE = "images/buttons.png";

const SDL_Rect BUTTON_CUTS[] = {
{0, 0, 140, 50},
{140, 0, 140, 50},
{2 * 140, 0, 140, 50},
{3 * 140, 0, 140, 50},
{4 * 140, 0, 140, 50},
};

enum class ButtonType{
    Play,
    Quit,
    Resume,
    Menu,
    PlayAgain,
    None
};


const ButtonType BUTTON_TYPES[] = {
    ButtonType::Play,
    ButtonType::Quit,
    ButtonType::Resume,
    ButtonType::Menu,
    ButtonType::PlayAgain
};


class Button {
public:
    Button (const SDL_FRect &box, SDL_Texture* texture, const SDL_Rect& spriteCut, ButtonType type);
    void draw(SDL_Renderer *renderer) const;
    bool contains(int x, int y) const;
    ButtonType getType() const;
    SDL_Texture* getTexture() const;

private:
    SDL_FRect box_;
    SDL_Texture* texture_;
    SDL_Rect spriteCut_;
    ButtonType type_;
};


#endif // BUTTON_H
