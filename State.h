//
// Created by lpgau on 2026-02-06.
//

#ifndef DEER_INVADERS_STATE_H
#define DEER_INVADERS_STATE_H
#include <SDL3/SDL_pixels.h>
class Entity;


//state machine
enum class State {
Menu,
Game,
ScoreBoard,
Shop,
Credits,
Pause,
Quit,
};

enum class ArmeNiveau {
    Classic,
    Fire,
    Ice,
    Tbd,
};

//Le type de Bullet
class BulletType{
public:
    BulletType () = default;
    virtual ~BulletType() = default;

    virtual void Affect (Entity *entity);
    virtual SDL_Color GetColor () = 0;
};

//Si invalide
class InvalidBulletType : public BulletType{
public:
    InvalidBulletType() = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
};

//Couleur Bullet de base
class ClassicBulletType : public BulletType{
public:
    ClassicBulletType() = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
};
//Bullet Niveau 2(Amélioration -> Shop)
class FireBulletType : public BulletType{
public:
    FireBulletType () = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
};

//Bullet Niveau 3(Amélioration -> Shop)
class IceBulletType : public BulletType{
public:
    IceBulletType () = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
};

//Bullet Niveau 4(Amélioration -> Shop)
class TBDBulletType : public BulletType{
public:
    TBDBulletType () = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
};

//Bullet de la competence special (Bullets Multicolor)
class CompetenceSpecialBulletType : public BulletType{
    public:
    CompetenceSpecialBulletType () = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
};

#endif //DEER_INVADERS_STATE_H