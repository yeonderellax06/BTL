#ifndef PLATFORM
#define FLATFORM
#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 400;
const int PLATFORM_HEIGHT = 14;
const int PLATFORM_WIDTH = 68;

struct Platform {
    int x;
    int y;
    PLatform (int _x, int _y){
        x = _x;
        y = _y;
    }
int };

#endif // PLATFORM
