//
// Created by lpgau on 2026-02-08.
//

#include "Player.h"
#include "State.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_timer.h"
#include <cmath>

Player::Player(SDL_Texture *texture) {
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
    transform.position = (SDL_FPoint){960, 950};
    transform.size = (SDL_FPoint){90.0f, 90.0f};
    //ClassicBulletType par défaut
    currentWeapon = new ClassicBulletType();
    //Aucun shield par default
    currentShield = new NoShieldType();
    //aucun HP bonus oar default
    currentHpBoost = new NoHpType();

    //Le Type d'entity
    entityType = EntityType::Player;
    // memorise la position de départ
    originY = transform.position.y;

    //Texture Player
    texturePlayerShip = texture;
}

//destructeur
Player::~Player() {
    if (currentWeapon != nullptr) {
        delete currentWeapon;
        currentWeapon = nullptr;
    }
    if (currentHpBoost != nullptr) {
        delete currentHpBoost;
        currentHpBoost = nullptr;
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
    // APRÈS
    float effectiveMaxSpeed = maxSpeed;
    if (slowTimer > 0.0f) {
        effectiveMaxSpeed *= slowFactor;
        slowTimer -= deltaTime;
        if (slowTimer < 0.0f) {
            slowTimer  = 0.0f;
            slowFactor = 1.0f;
        }
    }
    if (movement.velocity.x >  effectiveMaxSpeed) movement.velocity.x =  effectiveMaxSpeed;
    if (movement.velocity.x < -effectiveMaxSpeed) movement.velocity.x = -effectiveMaxSpeed;

    if (recoilVelocity > 0.0f) {
        // pousse vers le bas
        transform.position.y += recoilVelocity * deltaTime;
        //pour revenir progressivement
        recoilVelocity -= recoilDecay * deltaTime;

        if (recoilVelocity <= 0.0f) {
            recoilVelocity = 0.0f;
            recoilReturning = true; // commence le retour
        }
    }

    if (recoilReturning) {
        // glisse vers originY
        float returnSpeed = 100.0f;
        float diff = originY - transform.position.y;

        if (std::abs(diff) <= returnSpeed * deltaTime) {
            transform.position.y = originY; // snap final
            recoilReturning = false;
        } else {
            transform.position.y += returnSpeed * deltaTime * (diff > 0 ? 1 : -1);
        }
    }

}

//Pour tirer
void Player::Shoot(std::vector<Entity *> &entity, SDL_FPoint dir) {
    SDL_Color BulletColor = currentWeapon->GetColor(); //Prend la couleur de l'arme actuel
    SDL_Texture* bulletTexture = currentWeapon->GetTexture();
    float centerX = transform.position.x + (transform.size.x / 2.0f) - 20.0f; // centre la balle fix avec texture
    float bulletY = transform.position.y - 16.0f;

    if (dynamic_cast<FireBulletType*>(currentWeapon)) {
        // ARME FIRE Double Cannon
        float decalage = 20.0f; //decalage pour tirer d'un autre angle
        // balle de gauche
        SDL_FPoint spawnGauche = { centerX - decalage, bulletY };
        entity.push_back(new Bullet(spawnGauche, dir, BulletColor, false, bulletTexture));

        // balle de droite
        SDL_FPoint spawnDroite = { centerX + decalage, bulletY };
        entity.push_back(new Bullet(spawnDroite, dir, BulletColor, false, bulletTexture));
    }//Pour ICE
    else if (dynamic_cast<IceBulletType*>(currentWeapon)) {
        //code
        float decalage = 25.0f;
        float hight = -10.0f;
        entity.push_back(new Bullet({ centerX, bulletY + hight},          { 0.f,      -1.f    }, BulletColor, false, bulletTexture)); // centre
        entity.push_back(new Bullet({ centerX - decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, false, bulletTexture)); // gauche droit
        entity.push_back(new Bullet({ centerX + decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, false, bulletTexture)); // droite droit
    }//Pour Gold Bullet
    else if (dynamic_cast<GoldBulletType*>(currentWeapon)) {
        float decalage = 50.0f;
        float smallDecalage = 25.0f;
        float hight = -10.0f;

        entity.push_back(new Bullet({ centerX- smallDecalage, bulletY + hight},          { 0.f,      -1.f    }, BulletColor, false, bulletTexture)); // centre
        entity.push_back(new Bullet({ centerX+ smallDecalage, bulletY + hight},          { 0.f,      -1.f    }, BulletColor, false, bulletTexture)); // centre
        entity.push_back(new Bullet({ centerX - decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, false, bulletTexture)); // gauche droit
        entity.push_back(new Bullet({ centerX + decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, false, bulletTexture)); // droite droit

    }//Pour la compétence special
    else if (dynamic_cast<CompetenceSpecialBulletType*>(currentWeapon)) {
        float decalage = 25.0f;
        constexpr float angleBullet = 0.577f; // l'angle de tir ~ 30 Degree

        entity.push_back(new Bullet({ centerX, bulletY },          { 0.f,      -1.f    }, BulletColor, true, bulletTexture)); // centre
        entity.push_back(new Bullet({ centerX - decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, true, bulletTexture)); // gauche droit
        entity.push_back(new Bullet({ centerX + decalage, bulletY },{ 0.f,      -1.f    }, BulletColor, true, bulletTexture)); // droite droit
        entity.push_back(new Bullet({ centerX, bulletY },          { -angleBullet, -1.f    }, BulletColor, true, bulletTexture)); // diag gauche 30°
        entity.push_back(new Bullet({ centerX, bulletY },          { angleBullet,  -1.f    }, BulletColor, true, bulletTexture)); // diag droite 30°
    }
    else {
        // arme de base
        SDL_FPoint spawnPoint = { centerX, bulletY };
        entity.push_back(new Bullet(spawnPoint, dir, BulletColor, false, bulletTexture));
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

void Player::ActivateCompetence(SDL_Texture* texture) {
    if (!bCompetenceReady || bCompetenceActive) return;

    bCompetenceActive = true;
    bCompetenceReady  = false;
    competenceActiveTimer = competenceActiveDuration;

    // Sauvegarder l'arme actuelle et équiper la compétence
    previousWeapon = currentWeapon; // pas de delete
    currentWeapon  = new CompetenceSpecialBulletType();
    currentWeapon->texture = texture;
}
//fonction si Un ennemie est detruit alors on ajoute au timer
void Player::AddKillToCompetence() {
    if (bCompetenceActive) return; // pas besoin si déjà active
    competenceTimer = std::min(competenceTimer + 2.0f, competenceCooldown);
    if (competenceTimer >= competenceCooldown)
        bCompetenceReady = true;
}

//Pour les bullets du joueur
Bullet::Bullet(SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color,bool isRGB, SDL_Texture *texture) {
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
    transform.size = (SDL_FPoint){40.f, 40.f};
    bIsRGB = isRGB;
    entityType = EntityType::Bullet;

    textureBullet = texture;
}
//Pour les missiles lancer du joueur
MissilePlayer::MissilePlayer(SDL_FPoint spawn, SDL_FPoint dir, SDL_Color color, SDL_Texture *texture) {
    AddComponent(MOVEMENT);
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
    transform.size = (SDL_FPoint){30.f, 60.f};
    entityType = EntityType::Bullet;

    //texture du missile
    textureMissile = texture;
}

//Tick cooldown des tires
void Player::ShootUpdate(std::vector<Entity *> &entity, SDL_FPoint dir, float deltaTime) {
    // Cooldown réduit de moitié si compétence active
    float activeCooldown = bCompetenceActive ? shootDefaultCooldown / 2.0f : shootDefaultCooldown;

    if (shootCooldown <= 0.f && isCurrentlyShooting) {
        Shoot(entity, dir);
        shootCooldown = activeCooldown; // <- utilise le bon cooldown

        if (!bCompetenceActive) {
            recoilVelocity = 150.0f;
        }
    } else {
        shootCooldown -= deltaTime * 150.f;
    }
}

bool Player::ArmeUpgrade(ArmeNiveau type, int &meatCount){
int weaponPrice = 0;

    switch (type) {
        case ArmeNiveau::Fire: weaponPrice = 25;
        break;
        case ArmeNiveau::Ice: weaponPrice = 50;
            break;
        case ArmeNiveau::Gold: weaponPrice = 250;
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

    // Creation de la nouvelle arme
    BulletType* newWeapon = nullptr;

    //On met l'etat qui correspond a l'enum
    switch (type) {
        case ArmeNiveau::Classic:
            newWeapon = new ClassicBulletType();
            break;
        case ArmeNiveau::Fire:
            newWeapon = new FireBulletType();
            break;
        case ArmeNiveau::Ice:
            newWeapon = new IceBulletType();
            break;
        case ArmeNiveau::Gold:
            newWeapon = new GoldBulletType();
            break;
    }

    if (bCompetenceActive) {
        // remplace previousWeapon lors de competence
        // (currentWeapon est la CompetenceSpecial, on la garde active)
        if (previousWeapon != nullptr) delete previousWeapon;
        previousWeapon = newWeapon;
    } else {
        // Comportement normal
        if (currentWeapon != nullptr) delete currentWeapon;
        currentWeapon = newWeapon;
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
        case ShieldAmount::SmallShield: shieldPrice = 25;
            break;
        case ShieldAmount::MediumShield: shieldPrice = 50;
            break;
        case ShieldAmount::LargeShield: shieldPrice = 250;
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

bool Player::HpUpgrade(HpAmount type, int &meatCount) {
    int HpPrice = 0;

    switch (type) {
        case HpAmount::SmallHpBonus: HpPrice = 25;
            break;
        case HpAmount::MediumHpBonus: HpPrice = 50;
            break;
        case HpAmount::LargeHpBonus: HpPrice = 250;
            break;
        case HpAmount::NoHpBonus: HpPrice = 0;
            break;
            default :HpPrice = 0;
    }

    //Si on a pas asser de viance pour acheter des HP -> On sort de la boucle
    if (meatCount < HpPrice) {
        SDL_Log("Pas asser de viande pour avoir davantage d'HP");
        return false; // Fin de la fonction bool
    }
    // Creation du nouveau boost HP
    HpType* newHpBoost = nullptr;
    switch (type) {
        case HpAmount::NoHpBonus:
            newHpBoost = new NoHpType();
            break;
        case HpAmount::SmallHpBonus:
            newHpBoost = new SmallHpType();
            break;
        case HpAmount::MediumHpBonus:
            newHpBoost = new MediumHpType();
            break;
        case HpAmount::LargeHpBonus:
            newHpBoost = new LargeHpType();
            break;
    }

    if (currentHpBoost != nullptr) delete currentHpBoost;
    currentHpBoost = newHpBoost;
    // Applique le bonus aux HP du joueur
    int bonus = currentHpBoost->GetBonusHP();
    health.current_health += bonus;
    health.max_health += bonus;

    meatCount -= HpPrice;
    SDL_Log("Boost HP acheter");
    return true;
}

//amilioration Missile joueur
bool Player::MissileUpgrade(MissileNiveau type, int &meatCount) {
    int missilePrice = 0;
    switch (type) {
        //prix missiles
        case MissileNiveau::SmallMissile:
            missilePrice = 25;
            break;
        case MissileNiveau::MediumMissile:
            missilePrice = 50;
            break;
        case MissileNiveau::LargeMissile:
            missilePrice = 250;
            break;
        case MissileNiveau::NoMissile:
            missilePrice = 0;
            break;
        default :missilePrice = 0;
    }
    //Si pas asser de viande alors on quitte la boucle
    if (meatCount < missilePrice) {
        SDL_Log("Impossible to buy a Missile");
        return false;
    }
    //Si on a deja un Missile
    if (currentMissile != nullptr) {
        delete currentMissile;
    }
    //creation des missiles
    switch (type) {
        case MissileNiveau::NoMissile:
            currentMissile = new InvalidMissileType();
            break;
        case MissileNiveau::SmallMissile:
            currentMissile = new SmallMissileType();
            break;
        case MissileNiveau::MediumMissile:
            currentMissile = new MediumMissileType();
            break;
        case MissileNiveau::LargeMissile:
            currentMissile = new LargeMissileType();
    }
    meatCount -= missilePrice;
    SDL_Log("New Missile Weapon unlocked");
    return true;//fin fonction bool
}


//Une Bombe kamikaz A une seule utilisation et qui coute 100 a chaque achat