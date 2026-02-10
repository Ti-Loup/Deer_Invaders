//
// Created by Louis-Philippe on 2/9/2026.
//

#include "Personnages.h"

#include "Components.h"

//enemy_Deer
Enemy_Deer::Enemy_Deer(float startX, float startY) {
    AddComponent (HEALTH);
    health.max_health = 100;
    health.current_health = 100;
    AddComponent (MOVEMENT);
    movement.velocity = { 0.0f,0.0f };
    AddComponent (RENDER);
    render.color = { 139, 69, 19, 255 };//couleur brun
    AddComponent (TRANSFORM);
    transform.position = { startX, startY };
    transform.size = { 100.f, 100.f };
}

Enemy_HealerDeer::Enemy_HealerDeer(float startX, float  startY) {

}