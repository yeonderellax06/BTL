#include "Platform.h"
#include <cstdlib>
#include <ctime>


using namespace std;
Platform::Platform(int _x, int _y){
    x = _x;
    y = _y;
}

SDL_Rect Platform::getRect() const{
    return SDL_Rect{x, y, PLATFORM_WIDTH, PLATFORM_HEIGHT};
}

PlatformManager::PlatformManager(int count, int spacingMin, int spacingMax)
: platformCount(count), verticalSpacingMin(spacingMin), verticalSpacingMax(spacingMax){
    srand(static_cast<unsigned> (time(0)));
    generatePlatforms();
}

void PlatformManager::generatePlatforms(){
    platforms.clear();
    int y = SCREEN_HEIGHT - 50;
    for (int i = 0; i < platformCount; i++){
        int x = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
        platforms.push_back(Platform(x,y));
        y -= verticalSpacingMin + rand() % (verticalSpacingMax - verticalSpacingMin);
    }
}

vector <Platform> &PlatformManager::getPlatforms(){
    return platforms;
}

void PlatformManager::update(float dy){
    for (auto&p : platforms){
        p.y +=dy;
        if (p.y > SCREEN_HEIGHT){
            p.y = 0;
            p.x = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
        }
    }
}

void PlatformManager::render(SDL_Renderer *renderer, SDL_Texture *platformTex){
    for (auto&p : platforms) {
        SDL_Rect rect = p.getRect();
        SDL_RenderCopy(renderer, platformTex, nullptr, &rect);
    }
}
