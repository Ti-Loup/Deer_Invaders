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

Uint32 lastBulletSpawned; // dernier bullet lancer
        Uint32 bulletRateSpawn;// delai entre 2 tirs


     //Le type d'arme (-> Dans State.h)
    BulletType* currentWeapon;

//fonctions

//fonction pour glisser
    void UpdatePhysics(float deltaTime);

    void Shoot(std::vector<Entity *> &entity, SDL_Point dir);
};

//Pour que le joueur puisse tirer
//Un point d'origine (SDL_FPoint et un vector ou le tire va vers)
class Bullet : public Entity{
    public:

    Bullet (SDL_FPoint spawn, SDL_Point dir, SDL_Color color);
};






#endif //DEER_INVADERS_PLAYER_H