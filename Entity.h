//
// Created by Louis-Philippe on 2/9/2026.
//

#ifndef DEER_INVADERS_ENTITY_H
#define DEER_INVADERS_ENTITY_H

#include "components.h"

#include <SDL3/SDL_render.h>//Pour Uint64
//Enum pour les différents types d'ennemies
enum class EntityType {
Player,//0
Enemy,//1
Bullet,//2
Shield,//3
EnemyBullet,//4
Collectable,//5
Particle //6
};

class Entity {
    // Le flag qui dit quels composants sont actifs
    Uint64 flag = 0u;

public:
    Entity() = default;
    virtual ~Entity() = default;//destruction
    //Pour la fonction update
    virtual void Update(float deltaTime) {}
    // Constructeur des EntityType
    //Collectible
    Entity(EntityType Collectible) : entityType(Collectible) {}
    // Les instances des componants
    //Les fonctions dans Components.h
    HealthComponents health;
    MovementComponents movement;
    RenderComponents render;
    TransformComponents transform;

    // Gestion des composants
    void AddComponent(Uint64 comps);
    bool HasComponent(Uint64 comps) const;
    void RemoveComponent(Uint64 comps);

    // Systèmes intégrés (Update et Render)
    virtual void MovementUpdate(float deltaTime);
    virtual void RenderUpdate(SDL_Renderer* renderer);
    virtual void HeightMovement(float deltaTime);

    //declaration des etiquettes de chacun pour intégrés les enums EntityType dans chaque entity
    EntityType entityType = EntityType::Enemy;

    //Bool pour savoir si un object est detruit ou non
    bool bIsDestroyed = false;
};

//La classe pour les 4 parties de la texture qui pars
class EnemyPart : public Entity{
public:
    SDL_Texture *texturePart = nullptr;
    float rotationAngle = 0.0f;
    float rotationSpeed = 200.0f;//rotation des textures cass/s
    float lifeTimer = 0.0f;
    float lifeDuration = 1.5f;//0.8 est nice et 4.0 aussi
//rect vide
SDL_FRect srcRect{};

    EnemyPart(SDL_FPoint pos, SDL_FRect src, SDL_FPoint velocity, SDL_Texture *texture, SDL_FPoint displaySize) : texturePart(texture), srcRect(src) {
        entityType = EntityType::Particle;
        transform.position = pos;
        transform.size = displaySize; // <- utilise la taille d'affichage
        movement.velocity = velocity;
    }
    void Update(float deltaTime) override {
        // Mouvement && rotation
        transform.position.x += movement.velocity.x * deltaTime;
        transform.position.y += movement.velocity.y * deltaTime;
        rotationAngle += rotationSpeed * deltaTime;
        // cmb de temps avant que les particles ce supp
        lifeTimer += deltaTime;
        if (lifeTimer >= lifeDuration) {
            bIsDestroyed = true;
        }
    }

};
//Pour les etoiles qui apparaient dans le menu etcc
class Stars : public Entity {
public:
//texture des etoiless
    SDL_Texture *texture = nullptr;

    //Pour la rotation droite et gauche
        float angle = 0.0f;
        float rotationSpeed = 90.0f;

    Stars(){
    entityType = EntityType::Particle;

     // grosseur aleatoire + position aleatoire
     float size = (float)(SDL_rand(30) + 15);
     transform.position.x = (float)SDL_rand(1920);
     transform.position.y = -(float)SDL_rand(1080);
     transform.size       = { size, size };

     // les gros etoiles descendent plus vite
     movement.velocity.y  = size * 1.5f + (float)SDL_rand(340);
     movement.velocity.x = size * 1.5f + (float)SDL_rand(20);
     rotationSpeed = (SDL_rand(2) == 0) ? 90.0f : -90.0f;

     Uint8 brightness = (Uint8)(80 + (size / 6.0f) * 175);
     render.color = { brightness, brightness, brightness, 255 };}
    void Update(float deltaTime) override {
            transform.position.y += movement.velocity.y * deltaTime;
            transform.position.x += movement.velocity.x * deltaTime;
            // respawn en haut
            if (transform.position.y > 1080 || transform.position.x > 1920) {
            transform.position.y = -5.0f;
            transform.position.x =  -400 +(float)SDL_rand(1920);
            }

            angle += rotationSpeed * deltaTime;
              if (angle >= 360.0f) angle -= 360.0f;
              if (angle < 0.0f)    angle += 360.0f;
    }
};


//Pour les etoiles types starwars durant le jeu
class GameStars : public Entity {
public:
    SDL_Texture *texture = nullptr;

    GameStars(){
        entityType = EntityType::Particle;

        float size = (float)(SDL_rand(4) + 1);
        transform.position.x = (float)SDL_rand(1920);
        transform.position.y = (float)SDL_rand(1080);
        transform.size = {size, size * 6.0f};

        movement.velocity.y = 100.0f + (size * 80.0f) + (float)SDL_rand(300);
        //color differente
        Uint8 brightness = (Uint8)(30 + (size / 5.0f) * 80);
        render.color = { brightness, brightness, brightness, 255 };
    }

    void Update(float deltaTime) override {
        transform.position.y += movement.velocity.y * deltaTime;

        if (transform.position.y > 1120) {
            transform.position.y = -10.0f;
            transform.position.x = (float)SDL_rand(1920);
        }
    }
};


#endif //DEER_INVADERS_ENTITY_H
