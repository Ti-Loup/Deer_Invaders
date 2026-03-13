//
// Created by lpgau on 2026-02-06.
//

#include "State.h"
#include "Main.h"


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
//Capacité spécial
void CompetenceSpecialBulletType::Affect(Entity *entity) {
    BulletType::Affect(entity);
}
SDL_Color CompetenceSpecialBulletType::GetColor () {

    return (SDL_Color){r, g, b, 255}; // A FAIRE RGB
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

//Pour le Shield Du Player
void ShieldType::Affect(Entity *entity) {

}

//No Shield (Transparant)
void NoShieldType::Affect(Entity *entity) {
    ShieldType::Affect(entity);
}
SDL_Color NoShieldType::GetColor () {
    return (SDL_Color){255, 255, 255, 0}; // <- Aucun shield alors opacité à 0
}
//Petit Shield (Bleu)
void SmallShieldType::Affect(Entity *entity) {
    ShieldType::Affect(entity);
}
SDL_Color SmallShieldType::GetColor () {
    return (SDL_Color){85,23,255,255};
}
//Moyen Shield (Mauve)
void MediumShieldType::Affect(Entity *entity) {
    ShieldType::Affect(entity);
}
SDL_Color MediumShieldType::GetColor () {
    return (SDL_Color){186, 23, 255, 255};
}
//Grand Shield (Jaune)
void LargeShieldType::Affect(Entity *entity) {
    ShieldType::Affect(entity);
}
SDL_Color LargeShieldType::GetColor () {
    return (SDL_Color){233,255,23,255};
}