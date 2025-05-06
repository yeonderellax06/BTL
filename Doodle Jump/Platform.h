#ifndef PLATFORM_H
#define PLATFORM_H

#include<SDL.h>
#include <vector>
#include <memory>
#include <ctime>
#include "constants.h"


class Platform {
protected:
    float x;
    int y;
public:
    Platform(int _x, int _y);
    virtual ~Platform() = default;

    virtual SDL_Rect getRect() const;
    virtual void update(float dy);
    virtual void render(SDL_Renderer* renderer, SDL_Texture* texture);
    virtual bool isBreakable() const;

    int getX() const { return x; }
    int getY() const { return y; }
    void setY(int _y) { y = _y; }
    void setX(int _x) { x = _x; }
};

class NormalPlatform : public Platform {
public:
    NormalPlatform(int x, int y);
};

class BreakablePlatform : public Platform {
private:
    bool isBroken;
    int breakTimer;
public:
    BreakablePlatform(int x, int y);
    int getBreakTimer() const { return breakTimer; }

    bool getIsBroken() const { return isBroken; }
    bool isBreakable() const override;
    void breakPlatform(); // Gọi khi player tiếp đất vào platform
    void update(float dy) override;
    void render(SDL_Renderer* renderer, SDL_Texture* texture) override;
};
class MoveablePlatform : public Platform {
private:
    int direction;
    float speed;
public:
    MoveablePlatform(int x, int y);
    void update(float dy) override;
    void updateHorizontal();
};

class PlatformManager {
private:
    std::vector<std::unique_ptr<Platform>> platforms;
    int platformCount;
    int verticalSpacingMin;
    int verticalSpacingMax;

public:
    PlatformManager(int count = 10, int spacingMin = 40, int spacingMax = 80);
    void generatePlatforms();
    std::vector<std::unique_ptr<Platform>>& getPlatforms();
    void update(float dy);
   void render(SDL_Renderer* renderer, SDL_Texture* normalTex, SDL_Texture* breakableTex, SDL_Texture* moveableTex, SDL_Texture* brokenTex);

    void reset();
};


#endif // PLATFORM_H
