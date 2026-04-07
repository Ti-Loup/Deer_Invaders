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
UpgradePopup, // Style vampire survivor
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
    Gold,
};
//Pour les differents types de shields

enum class ShieldAmount {
    NoShield,
    SmallShield,
    MediumShield,
    LargeShield,
};
//Pour les differents ameliorations des GetMaxShieldHP
enum class HpAmount{
NoHpBonus,
SmallHpBonus,
MediumHpBonus,
LargeHpBonus,
};

//Le type de Bullet
class BulletType{
public:
    BulletType () = default;
    virtual ~BulletType() = default;
    virtual void Affect (Entity *entity);
    virtual SDL_Color GetColor () = 0;
    virtual SDL_Texture *GetTexture() {return nullptr;} // valeur default
    SDL_Texture *texture = nullptr;//chaque bullet different garde sa propre texture

};

//Si invalide
class InvalidBulletType : public BulletType{
public:
    InvalidBulletType() = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
    SDL_Texture* GetTexture() override { return texture; }
};

//Couleur Bullet de base
class ClassicBulletType : public BulletType{
public:
    ClassicBulletType() = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
    SDL_Texture* GetTexture() override { return texture; }
};
//Bullet Niveau 2(Amélioration -> Shop)
class FireBulletType : public BulletType{
public:
    FireBulletType () = default;
    void Affect (Entity *entity) override;
//color & texture
    SDL_Color GetColor () override;
    SDL_Texture* GetTexture() override { return texture; }
};

//Bullet Niveau 3(Amélioration -> Shop)
class IceBulletType : public BulletType{
public:
    IceBulletType () = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
    SDL_Texture* GetTexture() override { return texture; }
};

//Bullet Niveau 4(Amélioration -> Shop)
class GoldBulletType : public BulletType{
public:
    GoldBulletType () = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
    SDL_Texture* GetTexture() override { return texture; }
};

//Bullet de la competence special (Bullets Multicolor)
class CompetenceSpecialBulletType : public BulletType{
    public:
    CompetenceSpecialBulletType () = default;
    void Affect (Entity *entity) override;

    SDL_Color GetColor () override;
    SDL_Texture* GetTexture() override { return texture; }
};
//Les differents state de Shield ( petit shield bleue, moyen shield mauve, grand shield Jaune)
class ShieldType {
    public:
    ShieldType () = default;
    virtual ~ShieldType() = default;

    virtual void Affect (Entity *entity);
    virtual SDL_Color GetColor () = 0;
    virtual int GetMaxShieldHP() = 0; //<- pour le nombre de pv du shield
};
//Pas de shield
class NoShieldType : public ShieldType {
public:
    NoShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor() override;
    int GetMaxShieldHP() override { return 0; } // si aucun shield alors 0
};

//Petit Shield
class SmallShieldType : public ShieldType {
    public:
    SmallShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
    int GetMaxShieldHP() override { return 50; }
};
//Moyen Shield
class MediumShieldType : public ShieldType {
public:
    MediumShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
    int GetMaxShieldHP() override { return 100; }
};
//Grand Shield
class LargeShieldType : public ShieldType {
public:
    LargeShieldType () = default;
    void Affect (Entity *entity) override;
    SDL_Color GetColor () override;
    int GetMaxShieldHP() override { return 250; }
};
// Les différents types de boost HP
class HpType {
public:
    HpType() = default;
    virtual ~HpType() = default;

    virtual void Affect(Entity* entity);
    virtual SDL_Color GetColor() = 0;
    virtual int GetBonusHP() = 0; // <- HP bonus accordé
};

class NoHpType : public HpType {
public:
    NoHpType() = default;
    void Affect(Entity* entity) override;
    SDL_Color GetColor() override;
    int GetBonusHP() override { return 0; }
};

class SmallHpType : public HpType {
public:
    SmallHpType() = default;
    void Affect(Entity* entity) override;
    SDL_Color GetColor() override;
    int GetBonusHP() override { return 50; }
};

class MediumHpType : public HpType {
public:
    MediumHpType() = default;
    void Affect(Entity* entity) override;
    SDL_Color GetColor() override;
    int GetBonusHP() override { return 100; }
};

class LargeHpType : public HpType {
public:
    LargeHpType() = default;
    void Affect(Entity* entity) override;
    SDL_Color GetColor() override;
    int GetBonusHP() override { return 250; }
};
#endif //DEER_INVADERS_STATE_H