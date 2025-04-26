#ifndef PLATFORM_H
#define PLATFORM_H

#include<SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "constants.h"

using namespace std;



struct Platform {
    int x,y;

    Platform (int _x, int _y);
    SDL_Rect getRect() const;
};
class PlatformManager {
private:
    vector <Platform> platforms;
    int platformCount;
    int verticalSpacingMin;
    int verticalSpacingMax;
public:
    PlatformManager(int count = 10, int spacingMin = 40, int spacingMax = 80);

    void generatePlatforms();
    vector <Platform> &getPlatforms();
    void update (float dy);
    void render (SDL_Renderer *renderer, SDL_Texture *platformTex);
    void reset();
};

#endif // PLATFORM_H
