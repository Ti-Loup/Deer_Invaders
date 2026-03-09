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
    Player ();
    ~Player();

     bool bIsMovingRight = false;
     bool bIsMovingLeft = false;


    bool isCurrentlyShooting = false;
  float shootCooldown = 0.f;
  float shootDefaultCooldown = 90.0f;//1.5 sec

     //Le type d'arme (-> Dans State.h)
    BulletType* currentWeapon;
BulletType* previousWeapon = nullptr; // pour restaurer l'arme après
//Competence special
float competenceTimer = 0.0f;
const float competenceCooldown = 100.0f; // <- vitesse du competenceCooldown
bool bCompetenceReady = false;
bool bCompetenceActive = false;
float competenceActiveDuration = 4.0f;
float competenceActiveTimer = 0.0f;


//fonctions

//fonction pour glisser
    void UpdatePhysics(float deltaTime);

    void Shoot(std::vector<Entity *> &entity, SDL_FPoint dir);

    void ShootUpdate (std::vector<Entity *> &entity, SDL_FPoint dir,float deltaTime);

    bool ArmeUpgrade(ArmeNiveau type, int &meatCount);

    //Competence Special
    void UpdateCompetence(float deltaTime);
    void ActivateCompetence();
    void AddKillToCompetence(); //ajout du temps si ennemi tuer

};

//Pour que le joueur puisse tirer
//Un point d'origine (SDL_FPoint et un vector ou le tire va vers)
class Bullet : public Entity{
    public:

    Bullet (SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color, bool bIsRGB = false);
    bool bIsRGB = false;//Pour que le rgb fonction (update la couleur a chaque frame)
};






#endif //DEER_INVADERS_PLAYER_H