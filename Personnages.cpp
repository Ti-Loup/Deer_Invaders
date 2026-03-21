//
// Created by Louis-Philippe on 2/9/2026.
//
#include <random>
#include "Personnages.h"

#include <algorithm>
#include <cstdlib>
#include <random>
#include <SDL3_image/SDL_image.h>

#include "Components.h"


//enemy_Deer
Enemy_Deer::Enemy_Deer(float startX, float startY, bool mouvementInverser, SDL_Texture *texture) {
    AddComponent (HEALTH);
    health.max_health = 40;

    health.current_health = 40;
    AddComponent (MOVEMENT);
    movement.velocity = { 50.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 100.f, 140.f };

    //Le type de l'entity
    entityType = EntityType::Enemy;
    //Position initiale
    this->startY = startY;
    if (mouvementInverser) {
        multiplicateurDirection = -1.0f;
    }else {
        multiplicateurDirection = 1.0f;
    }
    //temps avant de lancer leur premiere fraise
    this->strawberryCooldown = 1.0f + (static_cast<float>(rand() %9000) / 100.0f);

    textureCerf= texture;//assigne la texture au BulletStrawb -> deja assigner dans le constructeur main

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

    if (movementFunction != nullptr) {
        movementFunction(this, deltaTime);
    } else {

        timeAlive += deltaTime;
        float amplitude = 10.0f;
        float vitesse = 1.5f;
        transform.position.y = startY + (std::sin(timeAlive * vitesse) * amplitude * multiplicateurDirection);
    }



}
//Methode Update Method de EnemyDeer
void Enemy_Deer::Update(float deltaTime,std::vector<Entity*> &entities, SDL_Texture *texture) {
    HeightMovement(deltaTime); // appel de la fonction mouvement hauteur
    //timer pour tier
    strawberryCooldown -= deltaTime;
    //Quand timer == 0
    if (strawberryCooldown <= 0.0f) {
        StrawberryShoot(entities, {0,-1}, texture);
        //Temps avant qu'un cerf lance une fraise de nouveau
    this->strawberryCooldown = 8.0f + (static_cast<float>(rand() % 300) / 100.0f);
    }
    //Le flash
    if (bIsFlashing) {
        //timer diminue
        hitFlashTimer -= deltaTime;
        if (hitFlashTimer <= 0.0f) {
            hitFlashTimer = 0.0f;
            bIsFlashing = false;
        }
    }

}

//Nouvelle fonction pour faire tirer les fraises des cerfs vers le Joueur
void Enemy_Deer::StrawberryShoot(std::vector<Entity *> &entity, SDL_Point dir, SDL_Texture *texture ) {
    float spawnX = transform.position.x + (transform.size.x / 2.0f) - 10.0f;
    float spawnY = transform.position.y + transform.size.y;
    SDL_FPoint spawnPoint = {spawnX, spawnY};
    BulletStrawberry* bullet = new BulletStrawberry(spawnPoint, dir, texture);

    // Alterne le sens à chaque fraise
    bullet->rotationDirection = (strawberryCount % 2 == 0) ? 1.0f : -1.0f;
    strawberryCount++;

    entity.push_back(bullet);
}
//CERF HEALER
Enemy_HealerDeer::Enemy_HealerDeer(float startX, float  startY, SDL_Texture *texture) {
    AddComponent (HEALTH);
    health.max_health = 100;
    health.current_health = 100;
    AddComponent (MOVEMENT);
    movement.velocity = { 40.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 100.f, 140.f };

    //Le type d'entity
    entityType = EntityType::Enemy;

    textureDeerHealer = texture;
}
void Enemy_HealerDeer::Update(float deltaTime) {


    //Le flash rouge
    if (bIsFlashing) {
        //timer diminue
        hitFlashTimer -= deltaTime;
        if (hitFlashTimer <= 0.0f) {
            hitFlashTimer = 0.0f;
            bIsFlashing = false;
        }
    }
}

//CERF MAGE
Enemy_MageDeer::Enemy_MageDeer(float startX, float startY, SDL_Texture *texture) {
    AddComponent (HEALTH);
    health.max_health = 100;
    health.current_health = 100;
    AddComponent (MOVEMENT);
    movement.velocity = { 40.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 100.f, 140.f };

    //Le type d'entity
    entityType = EntityType::Enemy;

    //texture Mage Cerf
    textureDeerMage = texture;
}
void Enemy_MageDeer::Update(float deltaTime) {


    //Le flash Rouge
    if (bIsFlashing) {
        //timer diminue
        hitFlashTimer -= deltaTime;
        if (hitFlashTimer <= 0.0f) {
            hitFlashTimer = 0.0f;
            bIsFlashing = false;
        }
    }
}






Enemy_Meteor::Enemy_Meteor(float startX, float  startY, SDL_Texture *texture) {
    AddComponent (HEALTH);
    health.max_health = 40;
    health.current_health = 40;
    AddComponent (MOVEMENT);
    movement.velocity = { 0.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 100, 30, 255 };//couleur Meteor
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 80.f, 80.f };


    static std::random_device rd;
    static std::mt19937 gen(rd());//Pour un mouvement random
    std::uniform_real_distribution<float> dis(-60.0f, 60.0f);
    //genere sa vitesse sur l'axe des x
    movement.velocity.x = dis(gen);

    //Le type d'entity
    entityType = EntityType::Enemy;

    //Sa texture
    textureMeteor = texture;
}
//Update Method de la meteorite
void Enemy_Meteor::Update(float deltaTime) {
    float gravity = 200.0f;

    // Appliquer gravité
    movement.velocity.y += gravity * deltaTime;

    MovementUpdate(deltaTime);
    if (transform.position.y > 1080.0f) {
        bIsDestroyed = true;
    }

    //Le flash rouge
    if (bIsFlashing) {
        //timer diminue
        hitFlashTimer -= deltaTime;
        if (hitFlashTimer <= 0.0f) {
            hitFlashTimer = 0.0f;
            bIsFlashing = false;
        }
    }
}

//  BOSS

/**
 *Si le boss a > 100 hp il est en mode normal
 *Entre 50 et 75 il est enragee
 *Entre 25 et 50 il est tres facher et bouge differament
 *entre 0 et 25 (partie la plus difficile. mouvements rapide, tire rapide, laser rajouter)
 */
Enemy_FraiseBoss::Enemy_FraiseBoss(float startX, float  startY, SDL_Texture *texture) {
    AddComponent (HEALTH);
    health.max_health = 5000;
    health.current_health = 5000;
    AddComponent (MOVEMENT);
    movement.velocity = { 0.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 300.f, 400.f };

    //Le type d'entity
    entityType = EntityType::Enemy;
    textureBoss = texture;
}
    //Update method de Enemy_Boss Stage 1 et 2
void Enemy_FraiseBoss::Update(float deltaTime) {
    //Mouvement du boss
    //hp plus grand que 75% -> mode normal (facile)
    //velocity normal
    // Phase 1 100% à 75%
    if (health.current_health > 3750) {
        if (currentPhase != 1) {
            currentPhase = 1;
            bIsReturningCenter = true;
            movement.velocity.x = 30.0f;
            movement.velocity.y = 0.0f;
        }
    }
    // Phase 2 75% à 50%
    else if (health.current_health > 2500) {
        if (currentPhase != 2) {
            currentPhase = 2;
            bIsReturningCenter = true;
            movement.velocity.x = 60.0f;
            movement.velocity.y = 10.0f;
        }
    }
    // Phase 3 50% à 25%
    else if (health.current_health > 1250) {
        if (currentPhase != 3) {
            currentPhase = 3;
            bIsReturningCenter = true;
            movement.velocity.x = 100.0f;
            movement.velocity.y = 25.0f;
        }
    }
    // Phase 4 25% à 0%
    else {
        if (currentPhase != 4) {
            currentPhase = 4;
            bIsReturningCenter = true;
            movement.velocity.x = 150.0f;
            movement.velocity.y = 50.0f;
        }
    }

    // Bordures manuelles pour le boss
    if (transform.position.x <= 0.0f) {
        transform.position.x = 0.0f;
        movement.velocity.x = std::abs(movement.velocity.x); // rebond droite
    }
    else if (transform.position.x + transform.size.x >= 1920.0f) {
        transform.position.x = 1920.0f - transform.size.x;
        movement.velocity.x = -std::abs(movement.velocity.x); // rebond gauche
    }

    if (transform.position.y <= 0.0f) {
        transform.position.y = 0.0f;
        movement.velocity.y = std::abs(movement.velocity.y); // rebond bas
    }
    else if (transform.position.y + transform.size.y >= 700.0f) {
        transform.position.y = 700.0f - transform.size.y;
        movement.velocity.y = -std::abs(movement.velocity.y); // rebond haut
    }

    // Retour au centre
    if (bIsReturningCenter) {
        float centerX = 1920.0f / 2.0f - transform.size.x / 2.0f; //centre ecran
        float direction = (centerX - transform.position.x);
        if (std::abs(direction) > 5.0f) { // si pas encore au centre
            transform.position.x += (direction > 0 ? 1.0f : -1.0f) * 500.0f * deltaTime; // on retourne au centre
        } else {
            bIsReturningCenter = false;
        }
    }

    if (!bIsReturningCenter) {
        MovementUpdate(deltaTime);
    }
    //Le flash rouge
    if (bIsFlashing) {
        //timer diminue
        hitFlashTimer -= deltaTime;
        if (hitFlashTimer <= 0.0f) {
            hitFlashTimer = 0.0f;
            bIsFlashing = false;
        }
    }
}

//Missile
Missile::Missile(float startX, float startY, float playerX, SDL_Texture *texture) {
AddComponent (MOVEMENT);
movement.velocity = { 0.0f,-300.0f };
AddComponent (RENDER);
AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 30.0f, 50.0f };

    //le target est la derniere position du  joueur
    targetX = playerX;
    //texture
    textureMissile = texture;
    //type entity
    entityType = EntityType::EnemyBullet;
}
void Missile::Update(float deltaTime) {
//si les missiles vont vers le haut
    if (bIsGoingUp) {
    MovementUpdate(deltaTime);
        //si attein hauteurmax -> direction targetX (playerX)
        if (transform.position.y <= maxHeight) {
            bIsGoingUp = false;
            // direction -> joueur
            float dirX = targetX - transform.position.x;
            float dist = std::abs(dirX);//calcul distance entre positionX du joueur et le missile. (abs -> toujours positifff)
            movement.velocity.x = (dist > 0) ? (dirX / dist) * 200.0f : 0.0f;
            movement.velocity.y = 400.0f; // descend vers positionX joueur
        }
    }
    else {
        MovementUpdate(deltaTime);
        //destruction
        if (transform.position.y > 1100.0f) {
            bIsDestroyed = true;
        }
    }
}

//Laser
Laser::Laser(float posX, SDL_Texture *texture) {
    AddComponent (RENDER);

    AddComponent (TRANSFORM);

    //texture
    textureLaser = texture;
    //type entity
    entityType = EntityType::EnemyBullet;
}
void Laser::Update(float deltaTime) {

}




//Bullets
BulletStrawberry::BulletStrawberry(SDL_FPoint spawn, SDL_Point dir, SDL_Texture *texture) {
    AddComponent (MOVEMENT);
    movement.velocity = { 0.0f * dir.x,- 200.0f * dir.y };// les fraises vont vers le bas
    AddComponent (RENDER);
    render.color = { 255, 182, 193, 255 };//Pink for strawberry
    AddComponent (TRANSFORM);
    transform.position = spawn;
    transform.size = (SDL_FPoint){ 50.f, 50.f };
    entityType = EntityType::EnemyBullet;
    textureStrawb = texture;//assigne la texture au BulletStrawb -> deja assigner dans le constructeur main
}

//BulletStrawberry Update

void BulletStrawberry::Update(float deltaTime) {
    rotationAngle += rotationSpeed * rotationDirection * deltaTime;

    MovementUpdate(deltaTime);

    //detruit si les fraises sortent de l'ecran pour optimisation.
    if (transform.position.y > 1100.0f || transform.position.y < -50.0f) {
        bIsDestroyed = true;
    }
}
//  COLLECTIBLES

Collectible_Meat::Collectible_Meat(float startX, float startY, SDL_Texture *texture) {
    AddComponent (MOVEMENT);
    movement.velocity = { 0.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 255, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 40.f, 40.f };


    //Le type d'entity
    entityType = EntityType::Collectable;
//TEXTURE DU MEAT
    textureMeat = texture;
    //Rotation de depart aleatoire pour les meats entre 0 et 360
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dis(0.0f, 360.0f);
    rotationAngle = dis(gen); // ← angle de départ random
    rotationSpeed = 90.0f;
}
//MOUVEMENT DU COLLECTIBLE MEAT
void Collectible_Meat::Update(float deltaTime) {
    //Si vrai et qu'on a pas arreter sur le floor
    if (!bStoppedOnFloor) {
        rotationAngle += rotationSpeed * deltaTime;
    }

    const float gravity = 200.0f;
    const float floorY = 1020.0f; // adapte au sol réel
    static std::random_device rd;
    static std::mt19937 gen(rd());//Pour un rebond random sur l'axe des x

    // Appliquer gravité
    movement.velocity.y += gravity * deltaTime;

    // Appliquer mouvement
    MovementUpdate(deltaTime);

    // Collision sol
    if (transform.position.y >= floorY && movement.velocity.y > 0.0f)
    {
        transform.position.y = floorY;

        if (!bMeatHasTouchedGround)
        {
            std::uniform_real_distribution<float> dis(-45.0f, 45.0f);

            //génère le nombre
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

        }else
        {
            // Deuxième contact avec le sol → on arrête tout
            movement.velocity.y = 0.0f;
            movement.velocity.x = 0.0f;

            bStoppedOnFloor = true; //On arrete la rotation seulement au deuxieme toucher par terre
        }
    }



}