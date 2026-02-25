
//Les classes Player et Deers herite de Personnages

#ifndef DEER_INVADERS_PERSONNAGES_H
#define DEER_INVADERS_PERSONNAGES_H
#include "Entity.h"


class Personnages {

};


//Les types de cerfs

//Cerfs generaux
class Enemy_Deer : public Entity{
public:
    Enemy_Deer(float startX, float startY, bool mouvementInverser);

//  MOVEMENT CERFS VERTICAL
    void HeightMovement(float deltaTime) override;
    float startY;//depart
    float timeAlive = 0.f;
    float multiplicateurDirection = 1.0f;
};

//Les soigneurs
class Enemy_HealerDeer : public Entity{
public:
    Enemy_HealerDeer(float startX, float startY);
};
//Des meteorites qui tombent du ciel
class Enemy_Meteor : public Entity{
public:
    Enemy_Meteor(float startX, float startY);
};


//Bosses
class Enemy_FraiseBoss : public Entity {
public:
    Enemy_FraiseBoss(float floatX, float floatY);

};



//COLLECTIBLES
class Collectible_Meat : public Entity {
public:
    Collectible_Meat(float floatX, float floatY);
    //Update
    void Update(float deltaTime) override;
    //bool pour faire spawn un meatCollectible apres mort d'un cerf
    bool bMeatCollectibleCanSpawn = false;

    //bool pour savoir si chaque entity de CollectibleMeat touche le sol
    bool bMeatHasTouchedGround = false;

};

#endif //DEER_INVADERS_PERSONNAGES_H
