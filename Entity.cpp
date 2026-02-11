//
// Created by Louis-Philippe on 2/9/2026.
//

#include "Entity.h"

#include <random>

//Entity Conetient les components


void Entity::AddComponent(const Uint64 comps) {
    flag |= comps;
}

bool Entity::HasComponent(const Uint64 comps) const {
    return (flag & comps) == comps;
}

void Entity::RemoveComponent(const Uint64 comps) {
    flag &= ~comps;
}

void Entity::MovementUpdate(float deltaTime) {
    // Si l'entité a une position ET un mouvement, on bouge
    if (HasComponent(TRANSFORM | MOVEMENT)) {
        transform.position.x += movement.velocity.x * deltaTime;
        transform.position.y += movement.velocity.y * deltaTime;


        //Pour que les cerfs bouge légèrement haut et en bas de façon logique
        //Le randomizer <random>
       static std::random_device nombreAleatoire;
       static std::mt19937 aleatoire(nombreAleatoire());
       static std::uniform_real_distribution<float> distanceVitesse(-5.0f, 5.0f);
       static std::uniform_real_distribution<float> distanceTemp(1.0f, 3.0f); //Pendant combien de temps le cerf va bouger

        movement.Timer -= deltaTime;
        //Quand minuteur a 0
        if (movement.Timer <= 0.0f) {

            movement.velocity.y = distanceVitesse(aleatoire);//Bouge sur l'axe des y
            movement.Timer = distanceTemp(aleatoire);
        }


    }
}

void Entity::RenderUpdate(SDL_Renderer* renderer) {
    // Si l'entité a une position ET doit être rendue
    if (HasComponent(TRANSFORM | RENDER)) {
        SDL_FRect dst = {
            transform.position.x,
            transform.position.y,
            transform.size.x,
            transform.size.y
        };

        SDL_SetRenderDrawColor(renderer, render.color.r, render.color.g, render.color.b, render.color.a);
        SDL_RenderFillRect(renderer, &dst);
    }
}