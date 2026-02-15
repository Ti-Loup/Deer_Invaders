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
Collectable,//3
};

class Entity {
    // Le flag qui dit quels composants sont actifs
    Uint64 flag = 0u;

public:
    Entity() = default;
    virtual ~Entity() = default;//destruction

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

    //declaration des etiquettes de chacun pour intégrés les enums EntityType dans chaque entity
    EntityType entityType = EntityType::Enemy;

    //Bool pour savoir si un object est detruit ou non
    bool bIsDestroyed = false;


};



#endif //DEER_INVADERS_ENTITY_H
