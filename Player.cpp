//
// Created by lpgau on 2026-02-08.
//

#include "Player.h"
#include "State.h"
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
    float acceleration = 2000.0f; //acceleration vers -> MaxSpeed
    float friction = 1500.0f; //Temps que le personnage va continuer a bouger apres avoir relacher la touche clavier
    float maxSpeed = 700.0f; //Speed Max apres l'acceleration

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
    // 1. On calcule le centre comme d'habitude
    float centerX = transform.position.x + (transform.size.x / 2.0f) - 8.0f;
    float bulletY = transform.position.y - 16.0f;

    if (dynamic_cast<FireBulletType*>(currentWeapon)) {
        // ARME FIRE Double Cannon
        float decalage = 20.0f; //decalage pour tirer d'un autre angle
        // balle de gauche
        SDL_FPoint spawnGauche = { centerX - decalage, bulletY };
        entity.push_back(new Bullet(spawnGauche, dir, BulletColor));

        // balle de droite
        SDL_FPoint spawnDroite = { centerX + decalage, bulletY };
        entity.push_back(new Bullet(spawnDroite, dir, BulletColor));
    }//Pour ICE
    else if (dynamic_cast<IceBulletType*>(currentWeapon)) {
        //code
    }
    else if (dynamic_cast<TBDBulletType*>(currentWeapon)) {
        //code
    }
    else {
        // arme de base
        SDL_FPoint spawnPoint = { centerX, bulletY };
        entity.push_back(new Bullet(spawnPoint, dir, BulletColor));
    }
}

Bullet::Bullet(SDL_FPoint spawn, SDL_Point dir, SDL_Color color) {
    //Constructeur
    AddComponent(MOVEMENT);
    movement.velocity = (SDL_FPoint){0.f * dir.x, 700.f * dir.y}; //les bullets aillent vers le haut
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

bool Player::ArmeUpgrade(ArmeNiveau type, int &meatCount){
int weaponPrice = 0;

    switch (type) {
        case ArmeNiveau::Fire: weaponPrice = 10;
        break;
        case ArmeNiveau::Ice: weaponPrice = 50;
            break;
        case ArmeNiveau::Tbd: weaponPrice = 100;
            break;
        case ArmeNiveau::Classic: weaponPrice = 0;
            break;
        default: weaponPrice = 0;
            break;
    }
    //si pas asser de viande alors on quitte
    if (meatCount < weaponPrice) {
        SDL_Log("Pas asser de viande !");
        return false;//fin fonction bool
    }
    //Si on a deja une arme on la supprime pour en mettre une autre
    if (currentWeapon != nullptr) {
        delete currentWeapon;
    }
    //On met l'etat qui correspond a l'enum
    switch (type) {
        case ArmeNiveau::Classic:
            currentWeapon = new ClassicBulletType();
            break;
        case ArmeNiveau::Fire:
            currentWeapon = new FireBulletType();
            break;
        case ArmeNiveau::Ice:
            currentWeapon = new IceBulletType();
            break;
        case ArmeNiveau::Tbd:
            currentWeapon = new TBDBulletType();
            break;
    }

    //On fait la soustraction du nombre de viande Total - viande restante
    meatCount -= weaponPrice;
    SDL_Log("Nouvelle Arme debloquer");
    return true;//fin fonction bool
}
