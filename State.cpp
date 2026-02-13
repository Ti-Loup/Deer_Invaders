//
// Created by lpgau on 2026-02-06.
//

#include "State.h"

//Pour Le Bullet Du Player
void BulletType::Affect(Entity *entity) {

}

void InvalidBulletType::Affect(Entity *entity) {
    BulletType::Affect(entity);
}
SDL_Color InvalidBulletType::GetColor (){
    return (SDL_Color){0, 0, 0, 255};
}

//Bullet Classic
void ClassicBulletType::Affect(Entity *entity) {
    BulletType::Affect(entity);
}
SDL_Color ClassicBulletType::GetColor () {
    return (SDL_Color){255, 255, 255, 255};
}

//Bullet Niveau 2(Amélioration -> Shop)
void FireBulletType::Affect(Entity *entity) {
    BulletType::Affect(entity);
}
SDL_Color FireBulletType::GetColor () {
    return (SDL_Color){255, 0, 0, 255};
}

//Bullet Niveau 3(Amélioration -> Shop)
void IceBulletType::Affect(Entity *entity) {
    BulletType::Affect(entity);
}
SDL_Color IceBulletType::GetColor () {
    return (SDL_Color){175, 238, 238, 255};
}

//Bullet Niveau 4(Amélioration -> Shop)
void TBDBulletType::Affect(Entity *entity) {
    BulletType::Affect(entity);
}
SDL_Color TBDBulletType::GetColor () {
    return (SDL_Color){1, 2, 2, 255};
}