//
// Created by Louis-Philippe on 2/5/2026.
//

#ifndef DEER_INVADERS_MAIN_H
#define DEER_INVADERS_MAIN_H
#include <map>
#include <SDL3/SDL.h>

class Player;

class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

// Le extern permet de oartager une variable entre plusieurs fichier -> RGB du Main.cpp vers State.cpp
extern Uint8 r;
extern Uint8 g;
extern Uint8 b;

class Main {
//~ methodes ~
public:

void RenderTitle();//Pour mettre un titre
void RenderCreditsTitle();//Titre Credits
void RenderShopTitle();//Titre Shop
void RenderScoreTitle();//Titre Score
void UpdateBackgroundTint(const float deltaTime);
void Game();
//Ce qui permet d'associer une touche a une commande
Player* player = nullptr;
std::map<SDL_Scancode, Command*> keyBindings;
std::map<SDL_Scancode, Command*> keyReleaseBindings;


};



#endif //DEER_INVADERS_MAIN_H
