//
// Created by lpgau on 2026-02-08.
//

#include "Player.h"
#include "State.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_timer.h"
#include <cmath>

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
    //Aucun shield par default
    currentShield = new NoShieldType();

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
void Player::Shoot(std::vector<Entity *> &entity, SDL_FPoint dir) {
    SDL_Color BulletColor = currentWeapon->GetColor(); //Prend la couleur de l'arme actuel
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
        float decalage = 25.0f;
        float hight = -10.0f;
        entity.push_back(new Bullet({ centerX, bulletY + hight},          { 0.f,      -1.f    }, BulletColor)); // centre
        entity.push_back(new Bullet({ centerX - decalage, bulletY },{ 0.f,      -1.f    }, BulletColor)); // gauche droit
        entity.push_back(new Bullet({ centerX + decalage, bulletY },{ 0.f,      -1.f    }, BulletColor)); // droite droit
    }
    else if (dynamic_cast<TBDBulletType*>(currentWeapon)) {
        //code
    }//Pour la compétence special
    else if (dynamic_cast<CompetenceSpecialBulletType*>(currentWeapon)) {
        float decalage = 25.0f;
        constexpr float angleBullet = 0.577f; // l'angle de tir ~ 30 Degree

        entity.push_back(new Bullet({ centerX, bulletY },          { 0.f,      -1.f    }, BulletColor, true)); // centre
        entity.push_back(new Bullet({ centerX - decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, true)); // gauche droit
        entity.push_back(new Bullet({ centerX + decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, true)); // droite droit
        entity.push_back(new Bullet({ centerX, bulletY },          { -angleBullet, -1.f    }, BulletColor, true)); // diag gauche 30°
        entity.push_back(new Bullet({ centerX, bulletY },          { angleBullet,  -1.f    }, BulletColor, true)); // diag droite 30°
    }
    else {
        // arme de base
        SDL_FPoint spawnPoint = { centerX, bulletY };
        entity.push_back(new Bullet(spawnPoint, dir, BulletColor));
    }
}

void Player::UpdateCompetence(float deltaTime) {
    if (bCompetenceActive) {
        competenceActiveTimer -= deltaTime;
        if (competenceActiveTimer <= 0.0f) {
            // Fin de la compétence -> restaurer l'arme d'avant
            bCompetenceActive = false;
            competenceActiveTimer = 0.0f;
            competenceTimer = 0.0f;

            if (currentWeapon != nullptr) delete currentWeapon;
            // Si on avait une arme sauvegardée on la remet, sinon arme de base
            currentWeapon = (previousWeapon != nullptr)
                ? previousWeapon
                : new ClassicBulletType();
            previousWeapon = nullptr;
        }
    } else if (!bCompetenceReady) {
        competenceTimer += deltaTime;
        if (competenceTimer >= competenceCooldown) {
            competenceTimer = competenceCooldown;
            bCompetenceReady = true;
        }
    }
}

void Player::ActivateCompetence() {
    if (!bCompetenceReady || bCompetenceActive) return;

    bCompetenceActive = true;
    bCompetenceReady  = false;
    competenceActiveTimer = competenceActiveDuration;

    // Sauvegarder l'arme actuelle et équiper la compétence
    previousWeapon = currentWeapon; // on NE delete PAS ici
    currentWeapon  = new CompetenceSpecialBulletType();
}
//fonction si Un ennemie est detruit alors on ajoute au timer
void Player::AddKillToCompetence() {
    if (bCompetenceActive) return; // pas besoin si déjà active
    competenceTimer = std::min(competenceTimer + 2.0f, competenceCooldown);
    if (competenceTimer >= competenceCooldown)
        bCompetenceReady = true;
}


Bullet::Bullet(SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color,bool isRGB) {
    //Constructeur
    AddComponent(MOVEMENT);//movement de base
    // vitesse toujours 700 peu importe l'angle passé
    //<cmath>
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > 0.f) {
        dir.x /= length;
        dir.y /= length;
    }
    movement.velocity = { 700.f * dir.x, 700.f * dir.y };
    AddComponent(RENDER);
    render.color = color;
    AddComponent(TRANSFORM);
    transform.position = spawn;
    transform.size = (SDL_FPoint){16.f, 16.f};
    bIsRGB = isRGB;
    entityType = EntityType::Bullet;
}

float ShootCooldown = 0.f;
float ShootDefaultCooldown = 90.f;

//Tick cooldown des tires
void Player::ShootUpdate(std::vector<Entity *> &entity, SDL_FPoint dir, float deltaTime) {
    // Cooldown réduit de moitié si compétence active
    float activeCooldown = bCompetenceActive ? ShootDefaultCooldown / 2.0f : ShootDefaultCooldown;

    if (ShootCooldown <= 0.f && isCurrentlyShooting) {
        Shoot(entity, dir);
        ShootCooldown = activeCooldown; // <- utilise le bon cooldown
    } else {
        ShootCooldown -= deltaTime * 150.f;
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

//Fonction pour l'obtension d'un shield
//Si vrai on upgrade le shield
bool Player::ShieldUpgrade(ShieldAmount typeShield, int &meatCount) {
int shieldPrice = 0;
    switch (typeShield) {
        case ShieldAmount::SmallShield: shieldPrice = 10;
            break;
        case ShieldAmount::MediumShield: shieldPrice = 50;
            break;
        case ShieldAmount::LargeShield: shieldPrice = 100;
            break;
        case ShieldAmount::NoShield :shieldPrice = 0;
            break;
        default :shieldPrice = 0;
    }

    //Si on a pas asser de viance pour acheter un Shield -> On sort de la boucle
    if (meatCount < shieldPrice) {
        SDL_Log("Pas asser de viande pour acheter un shield");
        return false; // Fin de la fonction bool
    }
    //Si on a deja un shield
    if (currentShield != nullptr) {
        delete currentShield;
    }
    switch (typeShield) {
        case ShieldAmount::NoShield:
            currentShield = new NoShieldType();
            break;
        case ShieldAmount::SmallShield:
            currentShield = new SmallShieldType();
            break;
        case ShieldAmount::MediumShield:
            currentShield = new MediumShieldType();
            break;
        case ShieldAmount::LargeShield:
            currentShield = new LargeShieldType();
            break;
    }

    //On fait la soustraction du nombre de viande Total - viande restante
    maxShieldHP = currentShield->GetMaxShieldHP();//on vient chercher le nombre de pv du shield maximum du shield actuel
    currentShieldHP = maxShieldHP;
    meatCount -= shieldPrice;
    SDL_Log("Nouvelle Arme debloquer");
    return true;//fin fonction bool
}