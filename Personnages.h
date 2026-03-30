
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


    //Un flash quand le healer est toucher par bullet
      float hitFlashTimer = 0.0f; //le timer
      const float hitFlashDuration = 0.4f;//duree
      bool bIsFlashing = false;
};
//Les mages
class Enemy_MageDeer : public Entity {
    public:
    Enemy_MageDeer(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime, std::vector<Entity*> &entities);

    //texture du cerf Mage
   SDL_Texture  *textureDeerMage = nullptr;

 //Un flash quand le mage est toucher par bullet
   float hitFlashTimer = 0.0f; //le timer
   const float hitFlashDuration = 0.4f;//duree
   bool bIsFlashing = false;

   //pour la classe MagicBottle
   float magicShootTimer = 0.0f;
   float magicShootCooldown = 6.0f;

};

class Enemy_MageIceDeer : public Entity {
public:
    Enemy_MageIceDeer(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime, std::vector<Entity*> &entities);
    //texture du Mage Ice
    SDL_Texture *textureDeerMageIce = nullptr;

    //Un flash quand le mage est toucher par bullet
       float hitFlashTimer = 0.0f; //le timer
       const float hitFlashDuration = 0.4f;//duree
       bool bIsFlashing = false;

       //pour la classe MagicBottle
       float magicShootTimer = 0.0f;
       float magicShootCooldown = 6.0f;

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

//Barricade
class Enemy_Barricade : public Entity {
public:
    Enemy_Barricade(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime) override;

    //texture des barricades
    SDL_Texture *textureBarricade = nullptr;

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
    void Update(float deltaTime, std::vector<Entity*> &entities, float playerX, SDL_Texture* strawbTexture, SDL_Texture *missileTexture);
//Textures Cerf BOSS
    SDL_Texture *textureBoss = nullptr;

    //Un flash quand le cerf est toucher par bullet
    float hitFlashTimer = 0.0f; //le timer
    const float hitFlashDuration = 0.4f;//duree
    bool bIsFlashing = false;
    //boss different types de mouvements
    int currentPhase = 0;
    bool bPhaseInitialized = false;

    //Le timer des tire
    float shootTimer = 0.0f;
    float missileTimer = 0.0f;
    float laserTimer = 0.0f;

//Pour la retation des fraises
    int strawberryCount = 0;

   //pour qu'il retourne au centre entre chaque etat
   bool bIsReturningCenter = false;
};
//missile de boss
class Missile : public Entity {
public:
    Missile(float startX, float startY, float playerX, SDL_Texture* texture);
    void Update(float deltaTime) override;

    float targetX = 0.0f; //position du target
    bool bIsGoingUp = true;//position monte si vrai
    bool bWaiting = false; //attend en hauteur
    bool *loopFinished = nullptr; // partager entre les 3 missiles
    float maxHeight = 50.0f; //hauteur max

    //texture missile
    SDL_Texture *textureMissile = nullptr;

};

//laser de boss
class Laser : public Entity {
public:
    Laser (float posX, SDL_Texture* texture);
    void Update(float deltaTime) override;

//Le warning
bool bWarning = true; //indicateur ou laser
float warningTimer = 0.0f;
float warningDuration = 1.5f; // duree indicateur
//Le Laser
float laserWidth = 40.0f;//grosseur laser
float laserX = 0.0f; //position laser
float laserTimer = 0.0f;
float laserDuration = 0.5f; // duree du laser actif

    //texture Laser
    SDL_Texture *textureLaser = nullptr;


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
    bool bHasGravity = false;
        SDL_Texture *textureStrawb = nullptr; //creation texture

};
//La boule lancer d'un mage
class MagicBottle : public Entity {
public:
    MagicBottle(float startX, float startY);
    void Update(float deltaTime, std::vector<Entity*>& entities);

    //rotation de la bouteille
    float rotationAngle = 0.0f;
};
//La flaque de poison mauve sur le sol
class MagicPuddle : public Entity {
public:
    MagicPuddle(float startX, float startY);
    void Update(float deltaTime) override;

    float damageTimer = 0.0f;
    const float damageTick = 0.5f;//damage au sol
    const int damagePerTick = 15;
    float lifeTimer = 0.0f;
    const float lifeDuration = 5.0f;
    float alpha = 180.0f;
};

//La bouge de glace
class MagicIceCube : public Entity {
public:
    MagicIceCube(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime,  std::vector<Entity*>& entities);

    //Cube texture
    SDL_Texture *textureIceCube = nullptr;
    float splitY = 0.0f;        // Y aléatoire où le cube se divise
    bool bHasSplit = false;
};
//La boule de glace qui ce transforme en eclats
class MagicIceSnowflake : public Entity{
public:
    MagicIceSnowflake(float startX, float startY, SDL_Texture *texture = nullptr);
    void Update(float deltaTime,  std::vector<Entity*>& entities);

    //snowflake texture
    SDL_Texture *textureIceSnowflake = nullptr;

    float rotationAngle = 0.0f;
};
//Les eclats qui deviennent Puddle qui rallentissent le joueur sans faire de degats
class MagicIcePuddle : public Entity {
public:
    MagicIcePuddle(float startX, float startY);
    void Update(float deltaTime)override;


    float lifeTimer   = 0.0f;
    const float lifeDuration = 6.0f;
    float alpha = 180.0f;
    // pour que le joueur rallentissent
    const float slowFactor = 0.35f;
    const float slowDuration = 2.0f;
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
