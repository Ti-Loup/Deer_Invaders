
//Les classes Player et Deers herite de Personnages

#ifndef DEER_INVADERS_PERSONNAGES_H
#define DEER_INVADERS_PERSONNAGES_H
#include "Entity.h"
#include <functional>

class Personnages {

};


//Les types de cerfs

//Cerfs generaux
class Enemy_Deer : public Entity{
public:
    Enemy_Deer(float startX, float startY, bool mouvementInverser = false, SDL_Texture *texture = nullptr);

    void Update(float deltaTime, std::vector<Entity*> &entities, SDL_Texture *texture);
//  MOVEMENT CERFS VERTICAL
    void HeightMovement(float deltaTime) override;
//  CERFS TIRE DES FRAISES
    void StrawberryShoot(std::vector<Entity *> &entity, SDL_Point dir, SDL_Texture *texture );


    // Lambda de mouvement
    //Elle prend 2 parametres -> Enemy_Deer + float
    std::function<void(Enemy_Deer*, float)> movementFunction = nullptr;


    float startY;//depart
    float timeAlive = 0.f;
    float multiplicateurDirection = 1.0f;
    //Cooldown de fraises qui ce fait lacher par Cerf
    float strawberryCooldown = 2.0f;

   //Un flash quand le cerf est toucher par bullet
   float hitFlashTimer = 0.0f; //le timer
   const float hitFlashDuration = 0.4f;//duree
   bool bIsFlashing = false;
//Pour la retation des fraises
    int strawberryCount = 0;


float orbitAngle = 0.0f;

//Textures Cerf
    SDL_Texture *textureCerf = nullptr;
};

//Les soigneurs
class Enemy_HealerDeer : public Entity{
public:
    Enemy_HealerDeer(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update (float deltaTime)override;

    //texture du cerf Healer
    SDL_Texture *textureDeerHealer = nullptr;
};
//Les mages
class Enemy_MageDeer : public Entity {
    public:
    Enemy_MageDeer(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime);

    //texture du cerf Mage
   SDL_Texture  *textureDeerMage = nullptr;

};




//Des meteorites qui tombent du ciel
class Enemy_Meteor : public Entity{
public:
    Enemy_Meteor(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime) override;

    //Creation de la texture pour meteorite
    SDL_Texture *textureMeteor = nullptr;

    //Pour un indicateur si bullet touche au meteor
    //Un flash quand le cerf est toucher par bullet
    float hitFlashTimer = 0.0f; //le timer
    const float hitFlashDuration = 0.4f;//duree
    bool bIsFlashing = false;
};


//Bosses
class Enemy_FraiseBoss : public Entity {
public:
    Enemy_FraiseBoss(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime);
//Textures Cerf BOSS
    SDL_Texture *textureBoss = nullptr;

    //Un flash quand le cerf est toucher par bullet
    float hitFlashTimer = 0.0f; //le timer
    const float hitFlashDuration = 0.4f;//duree
    bool bIsFlashing = false;

};

//Bullets
class BulletStrawberry : public Entity {
     public:
        BulletStrawberry(SDL_FPoint spawn, SDL_Point dir, SDL_Texture *texture);
       void Update(float deltaTime);

    //ROTATION FRAISES
    float rotationAngle = 0.0f;
    float rotationSpeed = 180.0f; //La vitesse du mouvement rotatif
    float rotationDirection = 1.0f; //1 ou -1

        SDL_Texture *textureStrawb = nullptr; //creation texture


};

//COLLECTIBLES
class Collectible_Meat : public Entity {
public:
    Collectible_Meat(float floatX, float floatY, SDL_Texture *texture = nullptr);
    //Update
    void Update(float deltaTime) override;
    //bool pour faire spawn un meatCollectible apres mort d'un cerf
    bool bMeatCollectibleCanSpawn = false;

    //bool pour savoir si chaque entity de CollectibleMeat touche le sol
    bool bMeatHasTouchedGround = false;
    //Texture du meatCollectible
    SDL_Texture *textureMeat = nullptr;
    float rotationAngle = 0.0f;   // ← ajoute
    float rotationSpeed = 90.0f;
    bool bStoppedOnFloor = false;
};

#endif //DEER_INVADERS_PERSONNAGES_H
