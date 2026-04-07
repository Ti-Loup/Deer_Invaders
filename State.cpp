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
void GoldBulletType::Affect(Entity *entity) {
    BulletType::Affect(entity);
}
SDL_Color GoldBulletType::GetColor () {
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
}// HP Type (Base)
void HpType::Affect(Entity *entity) {

}

// No HP
void NoHpType::Affect(Entity *entity) {
    HpType::Affect(entity);
}
SDL_Color NoHpType::GetColor() {
    return (SDL_Color){255, 255, 255, 0};
}

// Small HP Boost (Vert pâle)
void SmallHpType::Affect(Entity *entity) {
    HpType::Affect(entity);
}
SDL_Color SmallHpType::GetColor() {
    return (SDL_Color){255, 255, 255, 0};
}

// Medium HP Boost (Vert)
void MediumHpType::Affect(Entity *entity) {
    HpType::Affect(entity);
}
SDL_Color MediumHpType::GetColor() {
    return (SDL_Color){255, 255, 255, 0};
}

// Large HP Boost (Vert vif)
void LargeHpType::Affect(Entity *entity) {
    HpType::Affect(entity);
}
SDL_Color LargeHpType::GetColor() {
    return (SDL_Color){255, 255, 255, 0};
}
//Missile Player
void MissileType::Affect(Entity *entity) {

}
void InvalidMissileType::Affect(Entity *entity) {
    MissileType::Affect(entity);
}
SDL_Color InvalidMissileType::GetColor() {
    return (SDL_Color){255, 234, 43, 0};//opaciter 0 si Invalide
}
//petitMissile
void SmallMissileType::Affect(Entity *entity) {
    MissileType::Affect(entity);
}
SDL_Color SmallMissileType::GetColor() {
    return (SDL_Color){254, 163, 71, 255};//couleur missile
}
//moyenMissile(+)
void MediumMissileType::Affect(Entity *entity) {
    MissileType::Affect(entity);
}
SDL_Color MediumMissileType::GetColor() {
    return (SDL_Color){250, 164, 1, 255};//Couleur missile
}
//LargeMissile(++)
void LargeMissileType::Affect(Entity *entity) {
    MissileType::Affect(entity);
}
SDL_Color LargeMissileType::GetColor() {
    return (SDL_Color){222, 152, 22, 255};//couleur missile
}