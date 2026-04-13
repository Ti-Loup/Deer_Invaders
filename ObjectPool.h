//
// Created by Louis-Philippe on 4/13/2026.
//

#ifndef DEER_INVADERS_OBJECTPOOL_H
#define DEER_INVADERS_OBJECTPOOL_H

#include "Player.h"
#include <vector>
#include <cmath>

class BulletPool {
public:
//le nombre de balles
    static constexpr int POOL_SIZE = 200;

    BulletPool(const BulletPool&) = delete;
    void operator=(const BulletPool&) = delete;

    static BulletPool& GetInstance() {
        static BulletPool instance;
        return instance;
    }

    //recupere une balle + initialization
    Bullet* Get(SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color, bool isRGB, SDL_Texture* texture);
    void Return(Bullet* bullet);

    int GetAvailableCount() const { return (int)available.size(); }

private:
    BulletPool();
    ~BulletPool();
    //toutes les balles + ceux available
    std::vector<Bullet*> all;
    std::vector<Bullet*> available;
};

#endif //DEER_INVADERS_OBJECTPOOL_H
