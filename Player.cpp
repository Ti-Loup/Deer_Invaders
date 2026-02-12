//
// Created by lpgau on 2026-02-08.
//

#include "Player.h"



Player::Player (){
    AddComponent(HEALTH);
    health.max_health = 250;
    health.current_health = health.max_health;
    AddComponent(MOVEMENT);
    movement.velocity = (SDL_FPoint){0.0f,0.0f};
    AddComponent(RENDER);
    render.color = (SDL_Color){125,125,125,125};
    AddComponent(TRANSFORM);
    //position depart
    transform.position = (SDL_FPoint){960,1000};
    transform.size = (SDL_FPoint){64.0f,64.0f};
}
//Pour modifier Sa vitesse, le freinage et Sa vitesse maximale
void Player::UpdatePhysics(float deltaTime) {


}

//void Player::Shoot (std::vector<Entity *> &entity, SDL_Point dir)
//{
//    entity.push_back (new Bullet (transform.position, dir, magic.type->GetColor ()));
//}

Bullet::Bullet (SDL_FPoint spawn, SDL_Point dir, SDL_Color color)
{
    AddComponent (MOVEMENT);
    movement.velocity = (SDL_FPoint){ 120.f * dir.x, 120.f * dir.y };
    AddComponent (RENDER);
    render.color = color;
    AddComponent (TRANSFORM);
    transform.position = spawn;
    transform.size = (SDL_FPoint){ 16.f, 16.f };
}