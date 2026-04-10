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
#include "Player.h"


//enemy_Deer
Enemy_Deer::Enemy_Deer(float startX, float startY, bool mouvementInverser, SDL_Texture *texture, bool stationary) {
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
    this->bIsStationary = stationary;
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
    //si true alors aucun mouvement
    if (bIsStationary) return;

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
    movement.velocity = { 50.0f,0.0f };
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
    movement.velocity = { 50.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 100.f, 140.f };

    //La frequence de lancer des potions random
    static std::mt19937 randomThrow(std::random_device{}());
    std::uniform_real_distribution<float> dis(0.0f, magicShootCooldown);
    magicShootTimer = dis(randomThrow);

    //Le type d'entity
    entityType = EntityType::Enemy;

    //texture Mage Cerf
    textureDeerMage = texture;
}
void Enemy_MageDeer::Update(float deltaTime, std::vector<Entity*> &entities) {
    //Rajoue de la mechanique de tire
    magicShootTimer += deltaTime;

    if (magicShootTimer >= magicShootCooldown) {
        magicShootTimer = 0.0f;

        // Nouveau cooldown random entre chaque tire
        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<float> disCooldown(8.0f, 14.0f);
        magicShootCooldown = disCooldown(gen);

        float spawnX = transform.position.x + transform.size.x / 2.0f - 12.5f;
        float spawnY = transform.position.y + transform.size.y;

        //ramdom du mouvement de bottle
        std::uniform_real_distribution<float> distanceX(-50.0f, 50.0f);
        std::uniform_real_distribution<float> distanceY(-150.0f, -80.0f);

        MagicBottle* bottle = new MagicBottle(spawnX, spawnY);
        bottle->movement.velocity.x = distanceX(gen);
        bottle->movement.velocity.y = distanceY(gen);
        entities.push_back(bottle);
    }

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

Enemy_MageIceDeer::Enemy_MageIceDeer(float startX, float startY, SDL_Texture *texture) {
    AddComponent (HEALTH);
    health.max_health = 100;
    health.current_health = 100;
    AddComponent (MOVEMENT);
    movement.velocity = { 50.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 255, 255, 255, 255 };
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 100.f, 140.f };

    //La frequence de lancer des morceaux de glace random
    static std::mt19937 randomThrow(std::random_device{}());
    std::uniform_real_distribution<float> dis(0.0f, magicShootCooldown);
    magicShootTimer = dis(randomThrow);

    //Le type d'entity
    entityType = EntityType::Enemy;

    //texture Mage Cerf
    textureDeerMageIce = texture;
}


void Enemy_MageIceDeer::Update(float deltaTime, std::vector<Entity *> &entities) {
    //Rajoue de la mechanique de tire
    magicShootTimer += deltaTime;

    if (magicShootTimer >= magicShootCooldown) {
        magicShootTimer = 0.0f;

        // Nouveau cooldown random entre chaque tire
        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<float> disCooldown(8.0f, 14.0f);
        magicShootCooldown = disCooldown(gen);

        float spawnX = transform.position.x + transform.size.x / 2.0f - 12.5f;
        float spawnY = transform.position.y + transform.size.y;

        //ramdom du mouvement des morceaux de glaces
        std::uniform_real_distribution<float> distanceX(-50.0f, 50.0f);
        std::uniform_real_distribution<float> distanceY(-150.0f, -80.0f);

        MagicIceCube* IceCube = new MagicIceCube(spawnX, spawnY);
        IceCube->movement.velocity.x = distanceX(gen);
        IceCube->movement.velocity.y = distanceY(gen);
        entities.push_back(IceCube);
    }

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
//CERF MELEE
Enemy_DeerMelee::Enemy_DeerMelee(float startX, float startY, SDL_Texture *texture) {
    AddComponent(HEALTH);
    health.max_health = 200;
    health.current_health = 200;
    AddComponent (MOVEMENT);
    movement.velocity = { 0.0f,0.0f };
    AddComponent(RENDER);
    render.color = { 255, 0, 0, 255 };
    AddComponent(TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 100.0f, 140.0f };

    //texture pour le cerf melee
    textureCerfMelee = texture;

    //Son type d'ennemi
    entityType = EntityType::Enemy;
}

void Enemy_DeerMelee::Update(float deltaTime, float playerX, float playerY) {
    //si charge
    if (!bIsCharging) {
        // le wait time
        waitTimer += deltaTime;
        if (waitTimer >= waitDuration) {
            bIsCharging = true;
        }
    } else {
        // direction vers joueur
        float dirX = playerX - transform.position.x;
        float dirY = playerY - transform.position.y;
        float length = std::sqrt(dirX * dirX + dirY * dirY);

        if (length > 0.0f) {
            movement.velocity.x = (dirX / length) * chargeSpeed;
            movement.velocity.y = (dirY / length) * chargeSpeed;
        }

        MovementUpdate(deltaTime);
    }

    // Flash rouge quand collision
    if (bIsFlashing) {
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

Enemy_Barricade::Enemy_Barricade(float startX, float startY, SDL_Texture *texture) {
    AddComponent (HEALTH);
    health.max_health = 500;
    health.current_health = 500;
    AddComponent (MOVEMENT);
    movement.velocity = { 50.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 237, 232, 208, 255 };//couleur Barricade (baige)
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 350.f, 30.f };

    //texture barricade
    textureBarricade = texture;

    //Le type d'entity
    entityType = EntityType::Enemy;

}

void Enemy_Barricade::Update(float deltaTime) {
    //Le flash jaune
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
void Enemy_FraiseBoss::Update(float deltaTime, std::vector<Entity*> &entities, float playerX, SDL_Texture *strawbTexture, SDL_Texture *missileTexture) {
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
    //Les 4 differentes phases de tires.
    //1 -> fraises
    //2 -> fraises++
    //3 -> missiles
    //4 -> Tous + laser
    shootTimer+=deltaTime;
    float centreX = transform.position.x + transform.size.x / 2.0f;
    if (currentPhase == 1 && shootTimer >= 2.0f) {
        shootTimer = 0.0f;//remet timer a 0
        //spawn fraise
        SDL_FPoint spawnFraise = {centreX - 25.0f, transform.position.y + transform.size.y};
        BulletStrawberry* fraise = new BulletStrawberry(spawnFraise, {0,-1}, strawbTexture);
        fraise->rotationDirection = (strawberryCount % 2 == 0) ? 1.0f : -1.0f; // ←rotation gauche ou droite de kla fraise
        strawberryCount++;
        entities.push_back(fraise);

    }
    else if (currentPhase == 2 && shootTimer >= 3.0f) {
        shootTimer = 0.0f;
        float centreX = transform.position.x + transform.size.x / 2.0f;
        float centreY = transform.position.y + transform.size.y;

        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<float> disX(-100.0f, 100.0f);
        std::uniform_real_distribution<float> disY(-200.0f, 50.0f); // certaines montent un peu avant de retomber

        for (int i = -1; i <= 1; i++) {
            SDL_FPoint spawn = { centreX + (i * 80.0f), centreY };
            BulletStrawberry* fraise = new BulletStrawberry(spawn, {0, -1}, strawbTexture);
            fraise->bHasGravity = true; // graviter pour les fraises du boss
            fraise->movement.velocity.x = disX(gen); // direction X aleatoire
            fraise->movement.velocity.y = disY(gen); // certaines montent avant de retomber
            fraise->rotationDirection = (strawberryCount % 2 == 0) ? 1.0f : -1.0f; // ←rotation gauche ou droite de kla fraise
            strawberryCount++;
            entities.push_back(fraise);
        }
    }
    else if (currentPhase == 3) {
        missileTimer += deltaTime;
        if (missileTimer >= 3.0f) {
            missileTimer = 0.0f;
            entities.push_back(new Missile(centreX - 100.0f, transform.position.y, playerX, missileTexture));
            entities.push_back(new Missile(centreX,          transform.position.y, playerX, missileTexture));
            entities.push_back(new Missile(centreX + 100.0f, transform.position.y, playerX, missileTexture));
        }
        // lasers
        laserTimer += deltaTime;
        if (laserTimer >= 4.0f) {
            laserTimer = 0.0f;
            for (int i = 0; i < 3; i++) {
                float laserX = static_cast<float>(rand() % 1800);
                entities.push_back(new Laser(laserX, nullptr));
            }
        }
    }
    else if (currentPhase == 4) {
        // fraises
        if (shootTimer >= 5.0f) {
            shootTimer = 0.0f;
            float centreX = transform.position.x + transform.size.x / 2.0f;
            float centreY = transform.position.y + transform.size.y;

            static std::mt19937 gen(std::random_device{}());
            std::uniform_real_distribution<float> disX(-100.0f, 100.0f);
            std::uniform_real_distribution<float> disY(-200.0f, 50.0f); // certaines montent un peu avant de retomber

            //spawn 2 fraises
            for (int i = -1; i <= 1; i++) {
                SDL_FPoint spawn = { centreX + (i * 80.0f), centreY };
                BulletStrawberry* fraise = new BulletStrawberry(spawn, {0, -1}, strawbTexture);
                fraise->bHasGravity = true; // graviter pour les fraises du boss
                fraise->movement.velocity.x = disX(gen); // direction X aleatoire
                fraise->movement.velocity.y = disY(gen); // certaines montent avant de retomber
                //rotation des fraises
                fraise->rotationDirection = (strawberryCount % 2 == 0) ? 1.0f : -1.0f; // ←rotation gauche ou droite de kla fraise
                strawberryCount++;
                entities.push_back(fraise);
            }
        }
        // missiles
        missileTimer += deltaTime;
        if (missileTimer >= 3.0f) {
            missileTimer = 0.0f;
            entities.push_back(new Missile(centreX - 100.0f, transform.position.y, playerX, missileTexture));
            entities.push_back(new Missile(centreX,          transform.position.y, playerX, missileTexture));
            entities.push_back(new Missile(centreX + 100.0f, transform.position.y, playerX, missileTexture));
        }
        // lasers
        laserTimer += deltaTime;
        if (laserTimer >= 4.0f) {
            laserTimer = 0.0f;
            for (int i = 0; i < 3; i++) {
                float laserX = static_cast<float>(rand() % 1800);
                entities.push_back(new Laser(laserX, nullptr));
            }
        }
    }
}

//Missile
Missile::Missile(float startX, float startY, float playerX, SDL_Texture *texture) {
AddComponent (MOVEMENT);
movement.velocity = { 0.0f,-150.0f };
AddComponent (RENDER);
AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 15.0f, 40.0f };

    //le target est la derniere position du  joueur
    targetX = playerX;
    //texture
    textureMissile = texture;
    //type entity
    entityType = EntityType::EnemyBullet;

    // Vitesse X et  de y aleatoire pour monter de facon irreguliere
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> disX(-150.0f, 150.0f);
    std::uniform_real_distribution<float> disY(-250.0f, -100.0f); // vitesse montee variable

    movement.velocity.x = disX(gen);
    movement.velocity.y = disY(gen);
}
void Missile::Update(float deltaTime) {

    if (bIsGoingUp) {
        // Gravite ralentit la montee progressivement
        movement.velocity.y += 200.0f * deltaTime;
        MovementUpdate(deltaTime);

        // Quand il ralentit assez -> vise le joueur
        if (movement.velocity.y >= 0.0f) {
            bIsGoingUp = false;
            // Imprecision aleatoire
            static std::mt19937 gen(std::random_device{}());
            std::uniform_real_distribution<float> dis(-250.0f, 250.0f);
            float imprecision = dis(gen);
            float dirX = (targetX + imprecision) - transform.position.x;
            float dirY = 1080.0f - transform.position.y;
            float length = std::sqrt(dirX * dirX + dirY * dirY);
            float speed = 500.0f;

            movement.velocity.x = (length > 0) ? (dirX / length) * speed : 0.0f;
            movement.velocity.y = (length > 0) ? (dirY / length) * speed : speed;
        }
    } else {
        // Gravite accelere la descente
        movement.velocity.y += 150.0f * deltaTime;
        MovementUpdate(deltaTime);
        if (transform.position.y > 1100.0f) {
            bIsDestroyed = true;
        }
    }
}

//Laser
Laser::Laser(float posX, SDL_Texture *texture) {
    AddComponent (RENDER);

    AddComponent (TRANSFORM);
    transform.position = { posX, 0.0f };
    transform.size = { laserWidth, 1080.0f };
    laserX = posX;
    //texture
    textureLaser = texture;
    //type entity
    entityType = EntityType::EnemyBullet;
    //indicateur color
    render.color = {255,0,0,80};
}
void Laser::Update(float deltaTime) {
//si warning -> timer commence
    //timer fini -> laser
    if (bWarning) {
        warningTimer += deltaTime;
        if (warningTimer >= warningDuration) {
            bWarning = false;
            //le laser qui tire
            render.color = { 255,0,0,255 };
        }
    }else {
        laserTimer += deltaTime;
        if (laserTimer >= laserDuration) {
            bIsDestroyed = true; // supp le laser apres
        }
    }
}




//Bullets & Enemy Attacks
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

    if (bHasGravity) {
        // Gravite pour les fraises du boss
        movement.velocity.y += 200.0f * deltaTime;
    }

    MovementUpdate(deltaTime);

    //detruit si les fraises sortent de l'ecran pour optimisation.
    if (transform.position.y > 1100.0f || transform.position.y < -50.0f) {
        bIsDestroyed = true;
    }
}

//Bouteille magie

MagicBottle::MagicBottle(float startX, float startY) {
    AddComponent(TRANSFORM);
    transform.position = {startX, startY};
    transform.size = {25.0f, 25.0f};

    AddComponent(MOVEMENT);
    movement.velocity = {0.0f, 200.0f};

    AddComponent(RENDER);
    render.color = {180, 0, 255, 255};

    entityType = EntityType::EnemyBullet;
}

void MagicBottle::Update(float deltaTime, std::vector<Entity*> &entities) {
    rotationAngle += 180.0f * deltaTime;
    //graviter
    movement.velocity.y += 200.0f * deltaTime;
    MovementUpdate(deltaTime);

    if (transform.position.y + transform.size.y >= 1000.0f) {
        entities.push_back(new MagicPuddle(transform.position.x - 50.0f,1000.0f));
        bIsDestroyed = true;
    }
    if (transform.position.y > 1100.0f) {
        bIsDestroyed = true;
    }
}

//la flaque de magie
MagicPuddle::MagicPuddle(float startX, float startY) {
    AddComponent(TRANSFORM);
    transform.position = {startX, startY};
    transform.size = {125.0f, 25.0f};

    AddComponent(RENDER);
    render.color = {150, 0, 255, 180};

    entityType = EntityType::EnemyBullet;
}

void MagicPuddle::Update(float deltaTime) {
    lifeTimer += deltaTime;
    damageTimer += deltaTime;

    if (lifeTimer > lifeDuration - 1.5f) {
        alpha = 180.0f * ((lifeDuration - lifeTimer) / 1.5f);
        render.color.a = (Uint8)alpha;
    }
    if (lifeTimer >= lifeDuration) {
        bIsDestroyed = true;
    }
}

// ICE MAGIC

//Ice Cube
MagicIceCube::MagicIceCube(float startX, float startY, SDL_Texture *texture) {
    AddComponent(TRANSFORM);
    transform.position = { startX, startY };
    transform.size     = { 45.0f, 45.0f };
    AddComponent(MOVEMENT);
    // movement aleatoire de depart
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> disX(-60.0f, 60.0f);
    std::uniform_real_distribution<float> disY(-180.0f, -80.0f);
    movement.velocity.x = disX(gen);
    movement.velocity.y = disY(gen);
    AddComponent(RENDER);
    render.color = { 180, 230, 255, 220 }; // couleur Ice

    entityType = EntityType::EnemyBullet;
    textureIceCube = texture;

    //descente vers le sol
    std::uniform_real_distribution<float> disSplit(500.0f, 650.0f);
    splitY = disSplit(gen);
}
void MagicIceCube::Update(float deltaTime,  std::vector<Entity*>& entities) {
    //graviter
    movement.velocity.y += 220.0f * deltaTime;
    MovementUpdate(deltaTime);
    //si on a pas encore split et que on est dans la zone pour split
    if (!bHasSplit && transform.position.y >= splitY) {
        bHasSplit = true;

        //random mouvement des
        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<float> disX(-120.0f, 120.0f);
        std::uniform_real_distribution<float> disY(-80.0f,  -20.0f);

        float positionX = transform.position.x + transform.size.x / 2.0f;
        float positionY = transform.position.y + transform.size.y / 2.0f;

        //3 snow flakes avec chaqun leur propre mouvement
        for (int i = 0; i < 3; i++) {
            auto* Snowflake = new MagicIceSnowflake(positionX - 8.0f, positionY - 8.0f);
            Snowflake->movement.velocity.x = disX(gen);
            Snowflake->movement.velocity.y = disY(gen);
            entities.push_back(Snowflake);
        }
        bIsDestroyed = true; // le cube disparait
    }

    if (transform.position.y > 1100.0f)
        bIsDestroyed = true;
}
//Snowflake (le cube mi chemain random Y ce brise en petit morceau en snowflake)
MagicIceSnowflake::MagicIceSnowflake(float startX, float startY, SDL_Texture *texture) {
    AddComponent(TRANSFORM);
    transform.position = { startX, startY };
    transform.size     = { 40.0f, 40.0f };
    AddComponent(MOVEMENT);
    AddComponent(RENDER);
    render.color = { 210, 240, 255, 200 };

    entityType = EntityType::EnemyBullet;
    //texture du Snowflake
    textureIceSnowflake = texture;
}
void MagicIceSnowflake::Update(float deltaTime, std::vector<Entity*>& entities) {
    //la texture qui rotate pour realisme
    rotationAngle += 120.0f * deltaTime;
    // Snowflake graviter < IceCube
    movement.velocity.y += 120.0f * deltaTime;
    MovementUpdate(deltaTime);

    // Spawn puddle au sol
    if (transform.position.y + transform.size.y >= 1000.0f) {
        float puddleX = transform.position.x + transform.size.x / 2.0f - 62.5f;
        entities.push_back(new MagicIcePuddle(puddleX, 1000.0f));
        bIsDestroyed = true;
    }
    if (transform.position.y > 1100.0f)
        bIsDestroyed = true;
}
//Ice Puddle
MagicIcePuddle::MagicIcePuddle(float startX, float startY) {
    AddComponent(TRANSFORM);
    transform.position = {startX, startY};
    transform.size = {125.0f, 25.0f};

    AddComponent(RENDER);
    render.color = {12,183,233, 180}; // couleur bleu clair

    entityType = EntityType::EnemyBullet;
}
void MagicIcePuddle::Update(float deltaTime) {
    lifeTimer += deltaTime;

    if (lifeTimer > lifeDuration - 1.5f) {
        alpha = 180.0f * ((lifeDuration - lifeTimer) / 1.5f);
        render.color.a = (Uint8)std::max(alpha, 0.0f);
    }
    if (lifeTimer >= lifeDuration)
        bIsDestroyed = true;
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