//
// Created by Louis-Philippe on 4/13/2026.
//
#include "ObjectPool.h"

BulletPool::BulletPool() {
    all.reserve(POOL_SIZE);
    available.reserve(POOL_SIZE);

    // prealloue toutes les balles au debut
    for (int i = 0; i < POOL_SIZE; i++) {
        Bullet* bullet = new Bullet({0.f, 0.f}, {0.f, -1.f},{255, 255, 255, 255}, false, nullptr);
        bullet->bIsDestroyed = true;
        all.push_back(bullet);
        available.push_back(bullet);
    }
}
//pour les deletes a la fin
BulletPool::~BulletPool() {
    for (auto* bullet : all) {
        delete bullet;
    }
}

Bullet* BulletPool::Get(SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color, bool isRGB, SDL_Texture* texture) {
    if (available.empty()) {
        // alloue une balle supplementaire si on a tout utiliser les 200 balles
        SDL_LogWarn(0, "BulletPool: pool épuisé, allocation dynamique");
        Bullet* bullet = new Bullet(spawn, dir, color, isRGB, texture);
        all.push_back(bullet); // ownership transférée au pool
        return bullet;
    }

    Bullet* bullet = available.back();
    available.pop_back();

    // Normalise la direction
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > 0.f) {
        dir.x /= length;
        dir.y /= length;
    }
    // Réinitialise la balle
    bullet->movement.velocity = {700.f * dir.x, 700.f * dir.y};
    bullet->render.color      = color;
    bullet->transform.position = spawn;
    bullet->transform.size    = {40.f, 40.f};
    bullet->bIsRGB            = isRGB;
    bullet->textureBullet     = texture;
    bullet->bIsDestroyed      = false;
    bullet->entityType        = EntityType::Bullet;
    return bullet;
}

void BulletPool::Return(Bullet* bullet) {
    bullet->bIsDestroyed  = true;
    bullet->textureBullet = nullptr;
    available.push_back(bullet);
}