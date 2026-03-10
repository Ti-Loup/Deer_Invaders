//
// Created by lpgau on 2026-02-06.
//

#ifndef DEER_INVADERS_STATE_H
#define DEER_INVADERS_STATE_H
#include <SDL3/SDL_pixels.h>
#include "Main.h"
class Entity;


//state machine
enum class State {
Menu,
ChoixNiveau,//Apres start pour choisir le niveau
Game,
IntroNiveau1,
IntroNiveau2,
IntroNiveau3,    //<- Pour l'intro
ScoreBoard,
Shop,
Credits,
Pause,
DeathScreen,
NiveauGagnerScreen,
Quit,
};

//State le type de wave
enum class WaveType{
Elimination,
Survival,
};
//Pour les differents types d'armes

enum class ArmeNiveau {
    Classic,
    Fire,
    Ice,
    Tbd,
};
//Pour les differents types de shields

enum class ShieldAmount {
    NoShield,
    SmallShield,
    MediumShield,
    LargeShield,
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
//Les differents state de Shield ( petit shield bleue, moyen shield mauve, grand shield Jaune)
class ShieldType {
    public:
    ShieldType () = default;
    virtual ~ShieldType() = default;

    virtual void Affect (Entity *entity);
    virtual SDL_Color GetColor () = 0;
};
class InvalidShieldType : public ShieldType {
    public:
    InvalidShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
};
//Pas de shield
class NoShieldType : public ShieldType {
public:
    NoShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor() override;
};

//Petit Shield
class SmallShieldType : public ShieldType {
    public:
    SmallShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
};
//Moyen Shield
class MediumShieldType : public ShieldType {
public:
    MediumShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
};
//Grand Shield
class LargeShieldType : public ShieldType {
public:
    LargeShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
};

#endif //DEER_INVADERS_STATE_H