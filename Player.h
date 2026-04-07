//
// Created by lpgau on 2026-02-08.
//

#ifndef DEER_INVADERS_PLAYER_H
#define DEER_INVADERS_PLAYER_H
#include "Entity.h"
#include "State.h"
#include <vector>

class Player : public Entity{
public:
    Player (SDL_Texture *texture = nullptr);
    ~Player();

     bool bIsMovingRight = false;
     bool bIsMovingLeft = false;
//l'effet du snow
    float slowTimer  = 0.0f;
    float slowFactor = 1.0f;


    bool isCurrentlyShooting = false;
    float recoilVelocity = 0.0f;      // vitesse de recul actuelle
    float recoilDecay = 600.0f;       // retour apres letire
    float originY = 0.0f;      // position Y de base
    bool recoilReturning = false; // phase de retour


  float shootCooldown = 0.f;
  float shootDefaultCooldown = 90.0f;//la vitesse de tire

     //Le type d'arme (-> Dans State.h)
    BulletType* currentWeapon;
BulletType* previousWeapon = nullptr; // pour restaurer l'arme après
    //Le type de shield de state.h
    ShieldType* currentShield;
    int currentShieldHP = 0; //HPSHIELd actuel
    int maxShieldHP = 0; // HP SHIELD MAX
    //le type de HP
    HpType* currentHpBoost;
        int currentHP = 0; //HP actuel
        int maxHP = 0; // HP MAX
    MissileType* currentMissile;
    MissileType* previousMissile = nullptr;


//Competence special
float competenceTimer = 0.0f;
const float competenceCooldown = 60.0f; // <- vitesse du competenceCooldown ~~ 30
bool bCompetenceReady = false;
bool bCompetenceActive = false;
float competenceActiveDuration = 4.0f; // combien de temps la capaciter dure
float competenceActiveTimer = 0.0f;

//texture du Player
   SDL_Texture  *texturePlayerShip = nullptr;

//fonctions

//fonction pour glisser
    void UpdatePhysics(float deltaTime);

    void Shoot(std::vector<Entity *> &entity, SDL_FPoint dir);

    void ShootUpdate (std::vector<Entity *> &entity, SDL_FPoint dir,float deltaTime);

    //Pour les boutons du shop, pouvoir changer l'arme et le shield en fonction du nombre de meat
    bool ArmeUpgrade(ArmeNiveau type, int &meatCount);
    bool ShieldUpgrade(ShieldAmount typeShield, int &meatCount);
    bool HpUpgrade(HpAmount type, int &meatCount);
    bool MissileUpgrade(MissileNiveau type, int &meatCount);
    //Competence Special
    void UpdateCompetence(float deltaTime);
    void ActivateCompetence(SDL_Texture* texture = nullptr);
    void AddKillToCompetence(); //ajout du temps si ennemi tuer

};

//Pour que le joueur puisse tirer
//Un point d'origine (SDL_FPoint et un vector ou le tire va vers)
class Bullet : public Entity{
    public:

    Bullet (SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color, bool bIsRGB = false, SDL_Texture *texture = nullptr);
    bool bIsRGB = false;//Pour que le rgb fonction (update la couleur a chaque frame)
    //la texture pour tous les bullets
    SDL_Texture *textureBullet = nullptr;

};

class MissilePlayer : public Entity {
public:
    MissilePlayer(SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color, SDL_Texture *texture = nullptr);

    //Texture des missiles
    SDL_Texture *textureMissile = nullptr;
};






#endif //DEER_INVADERS_PLAYER_H