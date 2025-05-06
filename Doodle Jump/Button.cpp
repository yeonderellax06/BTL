#include "Button.h"
#include "constants.h"

Button :: Button(const SDL_FRect&box, SDL_Texture* texture, const SDL_Rect&spriteCut, ButtonType type)
: box_(box), texture_(texture), spriteCut_(spriteCut), type_(type){}

void Button::draw(SDL_Renderer* renderer) const{
    SDL_Rect dest = {
        static_cast<int>(box_.x),
        static_cast<int>(box_.y),
        static_cast<int>(box_.w),
        static_cast<int>(box_.h)};
    SDL_RenderCopy(renderer, texture_,&spriteCut_,&dest);
}

bool Button::contains(int x, int y) const{
    return (x >= box_.x && x <= (box_.x + box_.w) &&
    y >= box_.y && y <= (box_.y + box_.h));
}

ButtonType Button::getType() const{
    return type_;
}

SDL_Texture* Button::getTexture() const{
    return texture_;
}
