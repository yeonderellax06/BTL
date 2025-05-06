#include "Platform.h"

Platform::Platform(int _x, int _y){
    x = static_cast<float>(_x);
    y = _y;
}

SDL_Rect Platform::getRect() const{
    return SDL_Rect{static_cast<int>(x), y, PLATFORM_WIDTH, PLATFORM_HEIGHT};
}

void Platform::update(float dy) {
    y += dy;
}

void Platform::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect rect = getRect();
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

bool Platform::isBreakable() const {
    return false;
}

NormalPlatform::NormalPlatform(int x, int y) : Platform(x, y) {};


bool BreakablePlatform::isBreakable() const {
    return true;
}

MoveablePlatform::MoveablePlatform(int x, int y) : Platform(x, y), direction(1), speed(0.8f) {}

void MoveablePlatform::update(float dy) {
    y += dy;
    updateHorizontal();
}

void MoveablePlatform::updateHorizontal() {
    x += direction * speed;

    if (x <= 0.0f) {
        x = 0.0f;
        direction = 1;
    } else if (x + PLATFORM_WIDTH >= SCREEN_WIDTH) {
        x = SCREEN_WIDTH - PLATFORM_WIDTH;
        direction = -1;
    }
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
        int type = rand()%3;

        switch (type) {
            case 0:
                platforms.push_back(std::make_unique<NormalPlatform>(x, y));
                break;
            case 1:
                platforms.push_back(std::make_unique<BreakablePlatform>(x, y));
                break;
            case 2:
                platforms.push_back(std::make_unique<MoveablePlatform>(x, y));
                break;
        }
        y -= verticalSpacingMin + rand() % (verticalSpacingMax - verticalSpacingMin);
    }
}

std::vector<std::unique_ptr<Platform>>& PlatformManager::getPlatforms() {
    return platforms;
}

void PlatformManager::update(float dy) {
    for (auto it = platforms.begin(); it != platforms.end(); ) {
        (*it)->update(dy);

        // Xóa platform vỡ sau khi hết hiệu ứng
        if (BreakablePlatform* bp = dynamic_cast<BreakablePlatform*>(it->get())) {
            if (bp->getIsBroken() && bp->getBreakTimer() <= 0) {
                it = platforms.erase(it);
                continue;
            }
        }

        // Respawn nếu rớt ra khỏi màn hình
        if ((*it)->getRect().y > SCREEN_HEIGHT) {
            int x = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
            int type = rand() % 3;
            switch (type) {
                case 0:
                    *it = std::make_unique<NormalPlatform>(x, 0);
                    break;
                case 1:
                    *it = std::make_unique<BreakablePlatform>(x, 0);
                    break;
                case 2:
                    *it = std::make_unique<MoveablePlatform>(x, 0);
                    break;
            }
        }

        ++it;
    }
}


void PlatformManager::render(SDL_Renderer *renderer, SDL_Texture *normalTex, SDL_Texture *breakableTex, SDL_Texture *moveableTex, SDL_Texture* brokenTex){
    for (auto& p : platforms) {
        if (BreakablePlatform* bp = dynamic_cast<BreakablePlatform*>(p.get())) {
            if (bp->getIsBroken())
                bp->render(renderer, brokenTex);
            else
                bp->render(renderer, breakableTex);
        }
        else if (dynamic_cast<MoveablePlatform*>(p.get()))
            p->render(renderer, moveableTex);
        else
            p->render(renderer, normalTex);
    }
}

BreakablePlatform::BreakablePlatform(int x, int y)
    : Platform(x, y), isBroken(false), breakTimer(0) {}

void BreakablePlatform::breakPlatform() {
    isBroken = true;
    breakTimer = 30; // Hiệu ứng vỡ kéo dài 30 frame
}

void BreakablePlatform::update(float dy) {
    Platform::update(dy);
    if (isBroken && breakTimer > 0)
        --breakTimer;
}

void BreakablePlatform::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    if (!isBroken || breakTimer > 0) {
        SDL_Rect rect = getRect();
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}


void PlatformManager::reset(){
    platforms.clear();
    generatePlatforms();
}
