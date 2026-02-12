//
// Created by lpgau on 2026-02-08.
//

#ifndef DEER_INVADERS_PLAYER_H
#define DEER_INVADERS_PLAYER_H
#include "Entity.h"
#include <vector>

class Player : public Entity{
public:
    Player ();
    //fonction pour glisser
     void UpdatePhysics(float deltaTime);
     bool bIsMovingRight = false;
     bool bIsMovingLeft = false;

    void Shoot(std::vector<Entity *> &entity, SDL_Point dir);
};

//Pour que le joueur puisse tirer
//Un point d'origine (SDL_FPoint et un vector ou le tire va vers)
class Bullet : public Entity{
    public:

    Bullet (SDL_FPoint spawn, SDL_Point dir, SDL_Color color);
};






#endif //DEER_INVADERS_PLAYER_H