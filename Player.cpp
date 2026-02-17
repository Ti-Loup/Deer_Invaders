//
// Created by lpgau on 2026-02-08.
//

#include "Player.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_timer.h"

Player::Player() {
    //constructeur
    AddComponent(HEALTH);
    health.max_health = 150;
    health.current_health = health.max_health;
    AddComponent(MOVEMENT);
    movement.velocity = (SDL_FPoint){0.0f, 0.0f};
    AddComponent(RENDER);
    render.color = (SDL_Color){125, 125, 125, 125};
    AddComponent(TRANSFORM);
    //position depart
    transform.position = (SDL_FPoint){960, 1000};
    transform.size = (SDL_FPoint){64.0f, 64.0f};
    //ClassicBulletType par défaut
    currentWeapon = new ClassicBulletType();

    //Le Type d'entity
    entityType = EntityType::Player;
}

//destructeur
Player::~Player() {
    if (currentWeapon != nullptr) {
        delete currentWeapon;
        currentWeapon = nullptr;
    }
}

//Pour modifier Sa vitesse, le freinage et Sa vitesse maximale
void Player::UpdatePhysics(float deltaTime) {
    float acceleration = 1000.0f; //acceleration vers -> MaxSpeed
    float friction = 400.0f; //Temps que le personnage va continuer a bouger apres avoir relacher la touche clavier
    float maxSpeed = 500.0f; //Speed Max apres l'acceleration

    //acceleration
    if (bIsMovingRight) {
        movement.velocity.x += acceleration * deltaTime;
    } else if (bIsMovingLeft) {
        movement.velocity.x -= acceleration * deltaTime;
    }

    //friction
    if (!bIsMovingRight && !bIsMovingLeft) {
        // Si on va à droite -> frein gauche
        if (movement.velocity.x > 0) {
            movement.velocity.x -= friction * deltaTime;
            if (movement.velocity.x < 0) movement.velocity.x = 0;
        }
        // Si on va à gauche -> frein droite
        else if (movement.velocity.x < 0) {
            movement.velocity.x += friction * deltaTime;
            if (movement.velocity.x > 0) movement.velocity.x = 0;
        }
    }

    //Pour pas depacer la vitesse maximal
    if (movement.velocity.x > maxSpeed) movement.velocity.x = maxSpeed;
    if (movement.velocity.x < -maxSpeed) movement.velocity.x = -maxSpeed;
}

//Pour tirer
void Player::Shoot(std::vector<Entity *> &entity, SDL_Point dir) {
    SDL_Color BulletColor = currentWeapon->GetColor(); //Prend la couleur de l'arme actuel
    //ce qui spawn
    entity.push_back(new Bullet(transform.position, dir, BulletColor));
}

Bullet::Bullet(SDL_FPoint spawn, SDL_Point dir, SDL_Color color) {
    //Constructeur
    AddComponent(MOVEMENT);
    movement.velocity = (SDL_FPoint){0.f * dir.x, 600.f * dir.y}; //les bullets aillent vers le haut
    AddComponent(RENDER);
    render.color = color;
    AddComponent(TRANSFORM);
    transform.position = spawn;
    transform.size = (SDL_FPoint){16.f, 16.f};

    entityType = EntityType::Bullet;
}

float ShootCooldown = 0.f;
float ShootDefaultCooldown = 90.f;

//Tick cooldown
void Player::ShootUpdate(std::vector<Entity *> &entity, SDL_Point dir, float deltaTime) {
    //Rajout si on appuie sur la touche
    if (ShootCooldown <= 0.f && isCurrentlyShooting == true) {
        // TIRER ICI
        Shoot(entity, dir);

        ShootCooldown = ShootDefaultCooldown;
    } else {
        ShootCooldown -= deltaTime *150.f;
    }
}
