//
// Created by lpgau on 2026-02-08.
//

#ifndef DEER_INVADERS_COMPONENTS_H
#define DEER_INVADERS_COMPONENTS_H
#include "state.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
/**
 * @headerfile components.h
 * @brief Ce module contient différentes classes représentant des types de composant. Ces classes
 * sont utilisées dans un style orienté objet pour représenter le patron de conception du composant
 * (component pattern).
 */

 //Creation du enum des components
 typedef enum
 {
 HEALTH = 1ULL << 0,
 MOVEMENT = 1ULL << 1,
 RENDER = 1ULL << 2,
 TRANSFORM = 1ULL << 3,
 } ComponentType;

//class de base
class Components {
public:
	virtual ~Components () = default;//destructions des components
};
//
class HealthComponents : public Components
{
  public:
	Sint32 current_health = 0;
	Sint32 max_health = 0;

	HealthComponents () = default;
};

class MovementComponents : public Components
{

  public:
	SDL_FPoint velocity = { 0.f, 0.f };
	MovementComponents () = default;
};

class RenderComponents : public Components
{
  public:
	SDL_Color color = { 0, 0, 0, 0 };

	RenderComponents () = default;
};

class TransformComponents : public Components
{

  public:
	SDL_FPoint position = { 0.f, 0.f };
	SDL_FPoint size = { 0.f, 0.f };

	TransformComponents () = default;
};
#endif //DEER_INVADERS_COMPONENTS_H