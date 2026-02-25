//
// Created by Louis-Philippe on 2/9/2026.
//
#include <random>
#include "Personnages.h"
#include <cstdlib>
#include <random>
#include "Components.h"

//enemy_Deer
Enemy_Deer::Enemy_Deer(float startX, float startY, bool mouvementInverser = false) {
    AddComponent (HEALTH);
    health.max_health = 40;
    health.current_health = 40;
    AddComponent (MOVEMENT);
    movement.velocity = { 50.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 80.f, 80.f };

    //Le type de l'entity
    entityType = EntityType::Enemy;
    //Position initiale
    this->startY = startY;
    if (mouvementInverser) {
        multiplicateurDirection = -1.0f;
    }else {
        multiplicateurDirection = 1.0f;
    }
/*
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
    */
}
//Ennemi Deer Monte et dessant progressivement
void Enemy_Deer::HeightMovement(float deltaTime) {

    timeAlive += deltaTime;

    float amplitude = 10.0f; //Amplitude du mouvement
    float vitesse = 1.5f;

    //nouvelle position Y
    transform.position.y = startY + (std::sin(timeAlive * vitesse) * amplitude * multiplicateurDirection);

}


Enemy_HealerDeer::Enemy_HealerDeer(float startX, float  startY) {
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

    //Le type d'entity
    entityType = EntityType::Enemy;
}

Enemy_Meteor::Enemy_Meteor(float startX, float  startY) {
    AddComponent (HEALTH);
    health.max_health = 60;
    health.current_health = 60;
    AddComponent (MOVEMENT);
    movement.velocity = { 30.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 80.f, 80.f };

    //Le type d'entity
    entityType = EntityType::Enemy;

}

//  BOSS
Enemy_FraiseBoss::Enemy_FraiseBoss(float startX, float  startY) {
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

    //Le type d'entity
    entityType = EntityType::Enemy;
}


//  COLLECTIBLES

Collectible_Meat::Collectible_Meat(float startX, float startY) {
    AddComponent (MOVEMENT);
    movement.velocity = { 0.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 255, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 18.f, 18.f };


    //Le type d'entity
    entityType = EntityType::Collectable;

}
//MOUVEMENT DU COLLECTIBLE MEAT
void Collectible_Meat::Update(float deltaTime) {
    const float gravity = 200.0f;
    const float floorY = 1050.0f; // adapte à ton sol réel
    static std::random_device rd;
    static std::mt19937 gen(rd());//Pour un rebond random sur l'axe des x (PEUT ETRE NEGATIF ET POSITIF )

    // Appliquer gravité
    movement.velocity.y += gravity * deltaTime;

    // Appliquer mouvement
    MovementUpdate(deltaTime);

    // Collision sol
    if (transform.position.y >= floorY)
    {
        transform.position.y = floorY;

        if (!bMeatHasTouchedGround)
        {
            std::uniform_real_distribution<float> dis(-45.0f, 45.0f);

            // 3. On génère le nombre
            movement.velocity.x = dis(gen);

            /*
            //Rajoue une petite velosité sur l'axe des x seulement negatif ou positif
            float randomMouvementX = -15.0f - (static_cast<float>(rand() % 30));

            movement.velocity.x = randomMouvementX;
            */
            //Rajoue un bon random
            float randomBounce = -100.0f - (static_cast<float>(rand() % 200));
            movement.velocity.y = randomBounce; // rebond
            bMeatHasTouchedGround = true;
        }
        else
        {
            movement.velocity.y = 0.0f; // stop complet
        }
    }
}