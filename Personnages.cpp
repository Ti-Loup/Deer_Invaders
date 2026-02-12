//
// Created by Louis-Philippe on 2/9/2026.
//
#include <random>
#include "Personnages.h"

#include "Components.h"

//enemy_Deer
Enemy_Deer::Enemy_Deer(float startX, float startY) {
    AddComponent (HEALTH);
    health.max_health = 100;
    health.current_health = 100;
    AddComponent (MOVEMENT);
    movement.velocity = { 30.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 80.f, 80.f };

    //Pour que les cerfs bouge légèrement haut et en bas de façon logique
    //Le randomizer <random>
    static std::random_device nombreAleatoire;
    static std::mt19937 aleatoire(nombreAleatoire());
    static std::uniform_real_distribution<float> distanceVitesse(-1.0f, 1.0f);
    static std::uniform_real_distribution<float> distanceTemp(1.0f, 1.0f); //Pendant combien de temps le cerf va bouger

    float deltaTime;
    movement.Timer -= deltaTime;
    //Quand minuteur a 0
    if (movement.Timer <= 0.0f) {

        movement.velocity.y = distanceVitesse(aleatoire);//Bouge sur l'axe des y
        movement.Timer = distanceTemp(aleatoire);
    }
}

Enemy_HealerDeer::Enemy_HealerDeer(float startX, float  startY) {

}