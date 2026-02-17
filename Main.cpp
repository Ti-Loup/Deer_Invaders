//defines
#define SDL_MAIN_USE_CALLBACKS//Pour le main


//includes
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "Main.h"
#include "State.h"
#include "Entity.h"
#include "Personnages.h"
#include "Player.h"


static constexpr Sint32 TILE_SIZE = 32;
static constexpr Sint32 ANIM_ROW_BEGIN = 0;
static constexpr Sint32 ANIM_ROW_END = 0;
static constexpr Sint32 ANIM_COL_BEGIN = 0;
static constexpr Sint32 ANIM_COL_END = 6;
static constexpr Sint32 PRESENT_SIZE = 8;

//Fonction FPS
static Uint32 TimerCallback(void *userdata, SDL_TimerID timerID, Uint32 interval) {
    bool *updateFlag = static_cast<bool *>(userdata);
    *updateFlag = true;
    return interval;
}


//Fonction pour que le tire ce fait chaque 2 sec
static Uint32 ShootCallback(void *userdata, SDL_TimerID timerID, Uint32 interval) {
    bool *updateFlag = static_cast<bool *>(userdata);
    *updateFlag = true;
    SDL_Log(0, "Timer Test");
    return interval;
}


Player *player;

class GameApp final {
public:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *spritesheet = nullptr;
    //Ajout du state
    State StateActuel = State::Menu;

    float animTimer = 0.0f;
    int currentCol = ANIM_COL_BEGIN;
    int currentRow = ANIM_ROW_BEGIN;
    const float frameDuration = 0.15f;

    float colorTime = 0.0f;
    Uint8 r = 0, g = 0, b = 0;

    //-> MENU <- Font et Texts
    TTF_Font *font = nullptr;
    TTF_Font *MenuFont = nullptr;
    TTF_Font *ShopFont = nullptr;
    TTF_TextEngine *textEngine = nullptr;
    TTF_Text *fpsText = nullptr;
    TTF_Text *MenuTitle = nullptr; //Pour rajouter un Titre
    TTF_Text *BoutonCreditsText = nullptr;

    //Texte special
    TTF_Font *MenuSpecialFont = nullptr;
    TTF_Text *MenuSpecialDraw = nullptr;

    //creation des boutons pour le menu
    SDL_FRect BoutonPlay = {50, 450, 250, 100};
    bool bClickedOnPlay = false;
    bool bClickedOnQuit = false;
    bool bClickedOnScore = false;
    bool bClickedOnShop = false;
    SDL_FRect BoutonScore = {120, 600, 250, 100};
    SDL_FRect BoutonQuit = {180, 750, 250, 100};
    SDL_FRect BoutonShop = {1350, 790, 170, 100};
    SDL_FRect BoutonCredits = {1350, 900, 170, 50};

    TTF_Text *TextStart = nullptr;
    TTF_Text *TextQuit = nullptr;
    TTF_Text *TextScore = nullptr; //Pour les boutons Start Quit et TBh
    TTF_Text *TextShop = nullptr;
    TTF_Text *TextQuitReturnMenu = nullptr;
    TTF_Font *BoutonFont = nullptr;

    // -> GAME <- Text et Fonts
    TTF_Font *InventoryFont = nullptr; // Game + Shop
    TTF_Text *InventoryText = nullptr; // Game + Shop


    // -> Score <- Text et Fonts
    TTF_Font *ReturnBoutonFont = nullptr;
    SDL_FRect BoutonQuitRetourMenu = {1600, 900, 200, 100};

    // -> Shop <- Text et Fonts
    //Fonts
    TTF_Font *BoutonUpgradeFont = nullptr;
    //Texts
    TTF_Text *BoutonUpgradeText = nullptr;
    TTF_Text *BoutonHPUpgradeText = nullptr;
    //Boutons
    SDL_FRect BoutonUpgrade = {300, 800, 125, 100};
    SDL_FRect BoutonHPUpgrade = {500, 800, 125, 100};

    // -> Credits <- Text et Fonts
    TTF_Font *CreditsFont = nullptr;
    TTF_Text *CreditsMenuText = nullptr;



    std::vector<float> frameTimes;
    const size_t MAX_SAMPLES = 100;
    bool shouldUpdateText = false;
    SDL_TimerID fpsTimerID;
    SDL_TimerID ShootTimerID;
    float currentFPS = 0.0f;

    float deltaTime = 0.f;

    // La liste de toutes les entités du jeu
    std::vector<Entity *> entities;

    //PATRON DE CONCEPTION SINGLETON
    //Point unique en avant du constructeur qui est devenue private
    static GameApp &GetInstance() {
        static GameApp instance;
        return instance;
    }

    GameApp(GameApp const &) = delete;

    void operator=(GameApp const &) = delete;

    //CONTROLLER
    SDL_Gamepad* gameController = nullptr; // Manette
    const Sint16 DEADZONE = 4000;          // Zone morte


private:
    GameApp() //Constructeur
    {
        //initionalisation du GAMEPAD
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) == false) {
            SDL_LogCritical(1, "SDL failed to initialize! %s", SDL_GetError());
            abort();
        }
        if (SDL_Init(SDL_INIT_VIDEO) == false) {
            SDL_LogCritical(1, "SDL failed to initialize! %s", SDL_GetError());
            abort();
        }
        window = SDL_CreateWindow("Deer Invaders", 1280, 720, 0);
        if (window == nullptr) {
            SDL_LogCritical(1, "SDL failed to create window! %s", SDL_GetError());
            abort();
        }
        //Mon renderer
        renderer = SDL_CreateRenderer(window, nullptr);
        if (renderer == nullptr) {
            SDL_LogCritical(1, "SDL failed to create renderer! %s", SDL_GetError());
            abort();
        }
        //Le fullscreen renderer qui s'adapte entre WindowMode et FullScreen
        SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX); //4 parameters


        spritesheet = IMG_LoadTexture(renderer, "assets/spritesheet.png");
        if (spritesheet == nullptr) {
            SDL_LogWarn(0, "SDL_image failed to load texture '%s'! %s", "assets/spritesheet.png",
                        SDL_GetError());
        }
        SDL_SetTextureScaleMode(spritesheet, SDL_SCALEMODE_NEAREST);
        if (TTF_Init() == false) {
            SDL_LogCritical(1, "SDL_ttf failed to initialize! %s", SDL_GetError());
            abort();
        }
        textEngine = TTF_CreateRendererTextEngine(renderer);
        if (textEngine == nullptr) {
            SDL_LogCritical(1, "SDL_ttf failed to create text engine!! %s", SDL_GetError());
            abort();
        }
        font = TTF_OpenFont("assets/font.ttf", 24);
        if (font == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to load font '%s'! %s", "assets/font.ttf", SDL_GetError());
        }
        fpsText = TTF_CreateText(textEngine, font, "FPS: 0", 20);
        if (fpsText == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to create text '%s'! %s", "FPS: 0", SDL_GetError());
        }
        if (TTF_SetTextColor(fpsText, 255, 255, 255, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        //Le font du Titre
        MenuFont = TTF_OpenFont("assets/New Space.ttf", 72);
        if (MenuFont == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to load font '%s'! %s", "assets/New Space.ttf", SDL_GetError());
        }
        //Rajoue du menu
        MenuTitle = TTF_CreateText(textEngine, MenuFont, "         Deer \nInvaders", 25);

        if (MenuTitle == nullptr) {
            SDL_LogWarn(0, "Le menu n'a pas chargé : TTF", SDL_GetError());
        }
        if (TTF_SetTextColor(MenuTitle, 255, 255, 255, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        //special font
        MenuSpecialFont = TTF_OpenFont("assets/Space.ttf", 120);
        if (MenuSpecialFont == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to put the font", SDL_GetError());
        }
        MenuSpecialDraw = TTF_CreateText(textEngine, MenuSpecialFont, "abcdefghigklmnopqrstuv", 20);

        BoutonFont = TTF_OpenFont("assets/New Space.ttf", 48);

        ReturnBoutonFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 44);
        ShopFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 60);
        if (ReturnBoutonFont == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        BoutonUpgradeFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 28);
        TextStart = TTF_CreateText(textEngine, BoutonFont, "Start", 25);
        if (MenuSpecialDraw == nullptr) {
            SDL_LogWarn(0, "Les boutons du menu n'a pas chargé : TTF", SDL_GetError());
        }
        TextQuit = TTF_CreateText(textEngine, BoutonFont, "Quit", 25);
        if (TextQuit == nullptr) {
            SDL_LogWarn(0, "Les boutons du menu n'a pas chargé : TTF", SDL_GetError());
        }
        TextScore = TTF_CreateText(textEngine, BoutonFont, "Score", 25);
        if (TextScore == nullptr) {
            SDL_LogWarn(0, "Les boutons du menu n'a pas chargé : TTF", SDL_GetError());
        }
        TextShop = TTF_CreateText(textEngine, ShopFont, "Shop", 25);
        if (TextShop == nullptr) {
            SDL_LogWarn(0, "Le text du bouton Shop n'a pas fonctionner", SDL_GetError());
        }
        BoutonUpgradeText = TTF_CreateText(textEngine, BoutonUpgradeFont, " Weapon \nUpgrade", 25);
        if (TTF_SetTextColor(BoutonUpgradeText, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "Couleur du bouton amelioration n'a pas fonctionné", SDL_GetError());
        }
        BoutonHPUpgradeText = TTF_CreateText(textEngine, BoutonUpgradeFont, "      HP \nUpgrade", 25);
        if (TTF_SetTextColor(BoutonHPUpgradeText, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "Couleur de Bouton HP Upgrade", SDL_GetError());
        }
        BoutonCreditsText = TTF_CreateText(textEngine, BoutonUpgradeFont, "Credits", 25);
        if (TTF_SetTextColor(BoutonCreditsText, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "Attention BoutonCredits n'a pas changer de couleurs", SDL_GetError());
        }


        if (TTF_SetTextColor(TextShop, 0, 0, 0, 250) == false) {
            SDL_LogWarn(0, "La couleur de TextShop n'a pas fonctionné", SDL_GetError());
        }
        if (TTF_SetTextColor(TextStart, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        if (TTF_SetTextColor(TextQuit, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        if (TTF_SetTextColor(TextScore, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        TextQuitReturnMenu = TTF_CreateText(textEngine, ReturnBoutonFont, "Return \nMenu", 25);
        if (TextQuitReturnMenu == nullptr) {
            SDL_LogWarn(0, "SDL_TTF failed to set the return Menu text)! %s", SDL_GetError());
        }
        if (TTF_SetTextColor(TextQuitReturnMenu, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set color TextQuitScore %s", SDL_GetError());
        }
        //credits
        CreditsFont = TTF_OpenFont("assets/ Cosmo Corner.ttf", 48);
        CreditsMenuText = TTF_CreateText(textEngine, CreditsFont, "Credits", 25);
        if (CreditsMenuText == nullptr) {
            SDL_LogWarn(0, "EERREUR ! Le texte CreditsText n'a pas charger", SDL_GetError());
        }

        // -> Dans Game <-

        //Joueur
        player = new Player();

        entities.push_back(player);

        //Ligne 1 de cerf

        entities.push_back(new Enemy_Deer(100.f, 50.0f));
        entities.push_back(new Enemy_Deer(250.f, 50.0f));
        entities.push_back(new Enemy_Deer(400.f, 50.0f));
        entities.push_back(new Enemy_Deer(550.0f, 50.0f));

        entities.push_back(new Enemy_Deer(700.0f, 50.0f));
        entities.push_back(new Enemy_Deer(850.0f, 50.0f));
        entities.push_back(new Enemy_Deer(1000.0f, 50.0f));
        entities.push_back(new Enemy_Deer(1150.0f, 50.0f));

        //Ligne 2 de cerfs
        entities.push_back(new Enemy_Deer(100.f, 180.0f));
        entities.push_back(new Enemy_Deer(250.f, 180.0f));
        entities.push_back(new Enemy_Deer(400.f, 180.0f));
        entities.push_back(new Enemy_Deer(550.0f, 180.0f));

        entities.push_back(new Enemy_Deer(700.0f, 180.0f));
        entities.push_back(new Enemy_Deer(850.0f, 180.0f));
        entities.push_back(new Enemy_Deer(1000.0f, 180.0f));
        entities.push_back(new Enemy_Deer(1150.0f, 180.0f));
        //Ligne 3 de cerfs
        entities.push_back(new Enemy_Deer(100.f, 330.0f));
        entities.push_back(new Enemy_Deer(250.f, 330.0f));
        entities.push_back(new Enemy_Deer(400.f, 330.0f));
        entities.push_back(new Enemy_Deer(550.0f, 330.0f));

        entities.push_back(new Enemy_Deer(700.0f, 330.0f));
        entities.push_back(new Enemy_Deer(850.0f, 330.0f));
        entities.push_back(new Enemy_Deer(1000.0f, 330.0f));
        entities.push_back(new Enemy_Deer(1150.0f, 330.0f));
        //Ligne 4 de cerfs
        entities.push_back(new Enemy_Deer(100.f, 480.0f));
        entities.push_back(new Enemy_Deer(250.f, 480.0f));
        entities.push_back(new Enemy_Deer(400.f, 480.0f));
        entities.push_back(new Enemy_Deer(550.0f, 480.0f));

        entities.push_back(new Enemy_Deer(700.0f, 480.0f));
        entities.push_back(new Enemy_Deer(850.0f, 480.0f));
        entities.push_back(new Enemy_Deer(1000.0f, 480.0f));
        entities.push_back(new Enemy_Deer(1150.0f, 480.0f));

        //Timer FPS
        fpsTimerID = SDL_AddTimer(250, TimerCallback, &shouldUpdateText);
    }

    //Libere memoire
    ~GameApp() {
        SDL_RemoveTimer(fpsTimerID);
        TTF_DestroyText(fpsText);
        TTF_DestroyText(MenuTitle);
        TTF_DestroyRendererTextEngine(textEngine);
        TTF_CloseFont(font);
        TTF_CloseFont(BoutonFont);
        TTF_CloseFont(CreditsFont);
        TTF_DestroyText(TextStart);
        TTF_DestroyText(TextQuit);
        TTF_DestroyText(TextScore);
        TTF_DestroyText(TextQuit);
        TTF_DestroyText(CreditsMenuText);
        TTF_CloseFont(MenuSpecialFont);
        SDL_DestroyTexture(spritesheet);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        if (gameController) {
            SDL_CloseGamepad(gameController);
        }
    }

    void CalculateFPS(const float deltaTime) {
        frameTimes.push_back(deltaTime);
        if (frameTimes.size() > MAX_SAMPLES) {
            frameTimes.erase(frameTimes.begin());
        }
        const float sum = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f);
        const float avgDelta = sum / static_cast<float>(frameTimes.size());
        currentFPS = (avgDelta > 0) ? 1.0f / avgDelta : 0;

        if (shouldUpdateText) {
            std::string fpsStr = "FPS: " + std::to_string(static_cast<int>(currentFPS));
            TTF_SetTextString(fpsText, fpsStr.c_str(), 0);
            shouldUpdateText = false; // Reset the flag
        }
    }

    void AdvanceAnimation(const float deltaTime) {
        animTimer += deltaTime;
        if (animTimer >= frameDuration) {
            animTimer = 0.0f;
            currentCol++;

            if (currentCol > ANIM_COL_END) {
                currentCol = ANIM_COL_BEGIN;
                currentRow++;
            }

            if (currentRow > ANIM_ROW_END) {
                currentRow = ANIM_ROW_BEGIN;
            }
        }
    }

    void RenderAnimation() const {
        if (spritesheet != nullptr) {
            const SDL_FRect src = {
                static_cast<float>(currentCol * TILE_SIZE),
                static_cast<float>(currentRow * TILE_SIZE),
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE),
            };

            constexpr SDL_FRect dst = {
                (3500.0f / 2.0f) - ((TILE_SIZE * PRESENT_SIZE) / 2.0f),
                (1700.0f / 2.0f) - ((TILE_SIZE * PRESENT_SIZE) / 2.0f),
                static_cast<float>((TILE_SIZE * PRESENT_SIZE)),
                static_cast<float>((TILE_SIZE * PRESENT_SIZE)),
            };

            SDL_RenderTexture(renderer, spritesheet, &src, &dst);
        }
    }


    //Petite fonction pour mettre un titre MENU
    void RenderTitle() {
        TTF_DrawRendererText(MenuTitle, 0, 200);
        TTF_DrawRendererText(MenuSpecialDraw, 000, 950);
    }



    //Boutons
    void RenderBoutons(const SDL_FRect &rect, TTF_Text *buttonText, Uint8 buttonr, Uint8 buttong, Uint8 buttonb) {
        SDL_SetRenderDrawColor(renderer, buttonr, buttong, buttong, 255);
        SDL_RenderFillRect(renderer, &rect);
        //Dessiner Texte au centre du boutton
        if (buttonText != nullptr) {
            int textW, textH; //Longeur/Largeur
            TTF_GetTextSize(buttonText, &textW, &textH);


            float textX = rect.x + (rect.w - textW) / 2.0f;
            float textY = rect.y + (rect.h - textH) / 2.0f;

            TTF_DrawRendererText(buttonText, textX, textY);
        }
    }

    void UpdateBackgroundTint(const float deltaTime) {
        constexpr float speed = 5.0f;
        colorTime += deltaTime * speed;

        constexpr float Amplitude = 60.0f;
        constexpr float MidPoint = 144.0f;

        r = static_cast<Uint8>(std::clamp(std::sin(colorTime) * Amplitude + MidPoint, 0.0f, 255.0f));
        g = static_cast<Uint8>(std::clamp(std::sin(colorTime + 2.0f) * Amplitude + MidPoint, 0.0f, 255.0f));
        b = static_cast<Uint8>(std::clamp(std::sin(colorTime + 4.0f) * Amplitude + MidPoint, 0.0f, 255.0f));
    }

    void RenderCreditsTitle() {
        TTF_DrawRendererText(CreditsMenuText, 200, 200);

    }


    //Menu du jeu qui run
    void Menu(float deltaTime) {
        SDL_Event MenuEvents;

        /*
         *Plus besoin des SDL_PoolEvent dans chaque classe
    while (SDL_PollEvent(&MenuEvents)) {
        if (MenuEvents.type == SDL_EVENT_QUIT) {
            StateActuel = State::Quit; // On change l'état vers Quit
        }

        if (MenuEvents.type == SDL_EVENT_MOUSE_BUTTON_DOWN && MenuEvents.button.button == SDL_BUTTON_LEFT) {
            SDL_FPoint MousePT = { MenuEvents.button.x, MenuEvents.button.y };

            // Si on clique sur PLAY
            if (SDL_PointInRectFloat(&MousePT, &BoutonPlay)) {
                SDL_Log("Transition vers le JEU");
                StateActuel = State::Game;
            }
            // Si on clique pour voir le score
            if (SDL_PointInRectFloat(&MousePT, &BoutonScore)) {
            SDL_Log("Transition vers Tableau Score");
                StateActuel = State::ScoreBoard;
            }
            // Si on clique sur QUIT
            if (SDL_PointInRectFloat(&MousePT, &BoutonQuit)) {
                StateActuel = State::Quit;
            }
        }
    }
    */
        UpdateBackgroundTint(deltaTime);
        // Rendu du menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderTitle();
        RenderAnimation();

        // Mise à jour animation
        AdvanceAnimation(deltaTime);

        // Dessin des boutons
        RenderBoutons(BoutonPlay, TextStart, 250, 255, 255);
        RenderBoutons(BoutonQuit, TextQuit, 250, 255, 255);
        RenderBoutons(BoutonScore, TextScore, 250, 255, 255);
        RenderBoutons(BoutonShop, TextShop, r, g, b);
        RenderBoutons(BoutonCredits, BoutonCreditsText, 255, 255, 255);


        TTF_DrawRendererText(fpsText, 1800, 10);
        SDL_RenderPresent(renderer);
    }

    // La fonction Game ne boucle
    void Game(float deltaTime) {
        SDL_Event GameEvents;
        /*
    *Plus besoin des SDL_PoolEvent dans chaque classe
    while (SDL_PollEvent(&GameEvents)) {
        if (GameEvents.type == SDL_EVENT_QUIT) {
            StateActuel = State::Quit;
        }
        //Les touches
    }
    */
        //Calcul de la nouvelle vitesse avant de deplacer les entities
        if (player != nullptr) {
            player->UpdatePhysics(deltaTime);
            player->ShootUpdate(entities, (SDL_Point){0, -1}, deltaTime);
        }
        //Variables de Detection des cerfs et murs
        bool ToucheMurGauche = false;
        bool ToucheMurDroit = false;
        float MaxPushBack = 0.0f;

        // Mouvement et Détection Des cerfs
        for (auto &ent: entities) {
            ent->MovementUpdate(deltaTime);

            if (ent->HasComponent(TRANSFORM)) {
                // Verifie Gauche
                if (ent->transform.position.x <= 0.0f) {
                    ToucheMurGauche = true;

                    // Calcul Combien le cerf est dans la bourdure
                    float deerInBorder = -ent->transform.position.x;


                    // Si le cerf est plus en dehors du screen c'est lui qui devient le lead
                    if (deerInBorder > MaxPushBack) {
                        MaxPushBack = deerInBorder;
                    }
                }
                // Verifie Droite
                else if ((ent->transform.position.x + ent->transform.size.x) >= 1920.0f) {
                    ToucheMurDroit = true;

                    float depassement = ent->transform.position.x + ent->transform.size.x - 1920.0f;

                    // Pareil pour la droite, on garde le plus grand dépassement
                    if (depassement > MaxPushBack) {
                        MaxPushBack = depassement;
                    }
                }
            }
        }

        // Application de la correction
        // touché à Gauche -> force vers la Droite
        if (ToucheMurGauche) {
            for (auto &ent: entities) {
                if (ent->HasComponent(TRANSFORM)) {
                    //vitesse en positif std::abs (droite)
                    ent->movement.velocity.x = std::abs(ent->movement.velocity.x);
                    ent->transform.position.x += (MaxPushBack + 1.0f);
                }
            }
        }
        // touché à Droite -> force vers la Gauche
        else if (ToucheMurDroit) {
            for (auto &ent: entities) {
                if (ent->HasComponent(TRANSFORM)) {
                    // vitesse en Negatif -std::abs (gauche)
                    ent->movement.velocity.x = -std::abs(ent->movement.velocity.x);
                    ent->transform.position.x -= (MaxPushBack + 1.0f);
                }
            }
        }

        //L'algorithme de collision
        //vérifier chaque balle pour voir si elle touche un ennemi.
        for (auto &bullet: entities) {
            //Si balle
            if (bullet->entityType != EntityType::Bullet) {
                //non
                continue;
            }
            for (auto &ennemi: entities) {
                if (ennemi->entityType == EntityType::Enemy) {
                    //oui (a coder)
                    //Creation Collision Bullet
                    SDL_FRect rectBullet = {
                        bullet->transform.position.x,
                        bullet->transform.position.y,
                        bullet->transform.size.x,
                        bullet->transform.size.y
                    };
                    //Creation Collision des ennemies
                    SDL_FRect rectEnnemi = {
                        ennemi->transform.position.x,
                        ennemi->transform.position.y,
                        ennemi->transform.size.x,
                        ennemi->transform.size.y
                    };
                    //Si collision entre Bullet & Ennemi ce fait -> SDL_HasRectIntersectionFloat (A,B) bool
                    if (SDL_HasRectIntersectionFloat(&rectBullet, &rectEnnemi)) {
                        SDL_LogWarn(0, "Collision fonctionne");
                        //detruit la balle au contact d'un cerf
                        bullet->bIsDestroyed = true;

                        //Pv des ennemies baisses
                        ennemi->health.current_health -= 20;
                        if (ennemi->health.current_health <= 0) {
                            ennemi->bIsDestroyed = true;
                            SDL_LogWarn(0, "Un cerf est mort");
                        }
                    }
                }
            }
        }


        // Rendu du jeu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir pour le jeu
        SDL_RenderClear(renderer);

        // Dessiner toutes les entités
        for (auto &ent: entities) {
            ent->RenderUpdate(renderer);
        }
        TTF_DrawRendererText(fpsText, 1800, 10); // Affiche FPS en jeu aussi

        SDL_RenderPresent(renderer);
    }

    //fonction pour la section score
    void Score(float deltaTime) {
        SDL_Event ScoreEvents;


        /*
         *Plus besoin des SDL_PoolEvent dans chaque classe
        while (SDL_PollEvent(&ScoreEvents)) {
            if (ScoreEvents.type == SDL_EVENT_MOUSE_BUTTON_DOWN && ScoreEvents.button.button == SDL_BUTTON_LEFT) {
                SDL_FPoint MousePT = { ScoreEvents.button.x, ScoreEvents.button.y };

                // Si on clique sur QUIT
                if (SDL_PointInRectFloat(&MousePT, &BoutonQuitScore)) {
                    StateActuel = State::Menu;
                }

            }

        }
        */
        // Rendu du Score
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);

        RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, 250, 255, 255);
        TTF_DrawRendererText(fpsText, 1800, 10); // Affiche FPS en jeu aussi

        SDL_RenderPresent(renderer);
    }

    //Avoir un shop pour acheter des skins -> amilioration d'arme
    void Shop(float deltaTime) {
        SDL_Event ShopEvents;

        //Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Fond noir
        SDL_RenderClear(renderer);

        RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, 255, 255, 255);
        RenderBoutons(BoutonUpgrade, BoutonUpgradeText, 255, 255, 255);
        RenderBoutons(BoutonHPUpgrade, BoutonHPUpgradeText, 255, 255, 255);
        //Fonts
        TTF_DrawRendererText(fpsText, 1800, 10);


        SDL_RenderPresent(renderer);
    }

    //Avoir un shop pour acheter des skins -> amilioration d'arme
    void Credits(float deltaTime) {
        SDL_Event ShopEvents;

        //Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Fond noir
        SDL_RenderClear(renderer);
        //boutons
        RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, 255, 255, 255);


        //Fonts
        TTF_DrawRendererText(fpsText, 1800, 10);


        SDL_RenderPresent(renderer);
    }

    //L'execution cera appeler par SDL a chaque frame au lieu du main ou on devait faire une boucle while pour faire la boucle Run a l'infini
public:
    //Void ne renvoie rien alors on utilise SDL_AppResult pour retourner SDL_APP_SUCCESS && SDL_APP_CONTINUE
    SDL_AppResult RunCallBacks() {
        static uint64_t lastTime = SDL_GetTicks();


        // Calcul du temps global
        const uint64_t currentTime = SDL_GetTicks();
        deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        CalculateFPS(deltaTime);

        switch (StateActuel) {
            case State::Menu:
                Menu(deltaTime);
                break;

            case State::Game:
                Game(deltaTime);
                break;

            case State::ScoreBoard:
                Score(deltaTime);
                break;
            //Pour ouvrir le shop
            case State::Shop:
                Shop(deltaTime);
                break;
            //Pour ouvrir Credits
            case State::Credits:
                Credits(deltaTime);
                break;

            case State::Quit:
                return SDL_APP_SUCCESS;
        }
        return SDL_APP_CONTINUE;
    }
};

//Appeler seulement une seul fois
//-> Parfait pour SINGLETON
SDL_AppResult
//deux *pour modifier et ecrire dans le pointeur (post-it de l'objet qu;on peut mettre de l'information a l'interieur)				2 etoiles argv signifie avec un array
SDL_AppInit(void **appstate, int argc, char *argv[]) {
    //Avec le SINGLETON ->
    GameApp::GetInstance();
    return SDL_APP_CONTINUE;
}

//Les events touches etc.
SDL_AppResult
SDL_AppEvent(void *appstate, SDL_Event *event) {
    //Le singleton Pour avoir les touches
    GameApp &app = GameApp::GetInstance();
    //MANETTE CONNECTER
    if (event->type == SDL_EVENT_GAMEPAD_ADDED) {
        if (app.gameController == nullptr) {
            app.gameController = SDL_OpenGamepad(event->gdevice.which);
            SDL_Log("Controller Detected");
        }
    }
    //MANETTE DECONNECTER
    if (event->type == SDL_EVENT_GAMEPAD_REMOVED) {
        if (app.gameController) {
            SDL_CloseGamepad(app.gameController);
            app.gameController = nullptr;
            SDL_Log("Controller Disconnected");
        }
    }
    // BOUTON MANETTE DOWN
    if (event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        //Pour tirer
        if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
            if (app.StateActuel == State::Game) {

                //POWER UP
            }
        }
    }
    // BOUTON MANETTE UP
    if (event->type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
        if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
            if (app.StateActuel == State::Game) {

                //RELACHE POWER UP
            }
        }
    }

    // MANETTE STICK
    if (event->type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
        // STICK GAUCHE AXE X
        if (event->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX) {
            Sint16 xValue = event->gaxis.value;

            if (app.StateActuel == State::Game) {
                // Vers la droite
                if (xValue > app.DEADZONE) {
                    player->bIsMovingRight = true;
                    player->bIsMovingLeft = false;
                }
                // Vers la gauche
                else if (xValue < -app.DEADZONE) {
                    player->bIsMovingRight = false;
                    player->bIsMovingLeft = true;
                }
                // deadZone
                else {
                    player->bIsMovingRight = false;
                    player->bIsMovingLeft = false;
                }
            }
        }
        else if (event->gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
            Sint16 triggerValue = event->gaxis.value;

            if (triggerValue > 3000) { // > deadZone
                if (app.StateActuel == State::Game) {
                    player->isCurrentlyShooting = true;
                }
            }
            else {
                // Si on relâche la gâchette
                if (app.StateActuel == State::Game) {
                    player->isCurrentlyShooting = false;
                }
            }
        }
    }




    //Si on clique sur le X de la fenêtre
    if (event->type == SDL_EVENT_QUIT) {
        app.StateActuel = State::Quit;
        return SDL_APP_CONTINUE;
    }

    //Gestion des touches de souris et le bouton
    //Mouse
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
        //Les variables
        float nouveauX, nouveauY;

        //Conversion de coordonnées des touches Lorsqu'on passe du plein écran au mode fenetrer
        SDL_RenderCoordinatesFromWindow(app.renderer, event->button.x, event->button.y, &nouveauX, &nouveauY);
        SDL_FPoint MousePT = {nouveauX, nouveauY};
        // On utilise les coordonnées converties pour créer le point

        // Dans le MENU
        if (app.StateActuel == State::Menu) {
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonPlay)) {
                app.StateActuel = State::Game;
            }
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonScore)) {
                app.StateActuel = State::ScoreBoard;
            }
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonShop)) {
                app.StateActuel = State::Shop;
            }
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonCredits)) {
                app.StateActuel = State::Credits;
            }
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonQuit)) {
                app.StateActuel = State::Quit;
            }
        }
        // Dans le Score
        else if (app.StateActuel == State::ScoreBoard) {
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonQuitRetourMenu)) {
                app.StateActuel = State::Menu;
            }
        }
        //Dans le Shop
        else if (app.StateActuel == State::Shop) {
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonQuitRetourMenu)) {
                app.StateActuel = State::Menu;
            }
            // Bouton Upgrade Arme
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonUpgrade)) {
                SDL_Log("Achat Upgrade Arme");
                // player->UpgradeWeapon();
            }

            // Bouton Upgrade HP
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonHPUpgrade)) {
                SDL_Log("Achat Upgrade HP");
                // player->Heal();
            }
        } else if (app.StateActuel == State::Credits) {
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonQuitRetourMenu)) {
                app.StateActuel = State::Menu;
            }
        }
    }
    //PRESS DOWN
    if (event->type == SDL_EVENT_KEY_DOWN) {
        //Si on est dans notre jeu alors on peut appuyer pour bouger notre personnage
        if (app.StateActuel == State::Game) {
            if (event->key.scancode == SDL_SCANCODE_D) {
                player->bIsMovingRight = true;
            }
            if (event->key.scancode == SDL_SCANCODE_A) {
                player->bIsMovingLeft = true;
            }
            //Pour tirer
            if (event->key.scancode == SDL_SCANCODE_SPACE) {
                player->isCurrentlyShooting = true;
            }
        }
        //DANS LE SHOP
        if (app.StateActuel == State::Shop) {
        }

        //Mettre le jeu plein ecran
        if (event->key.scancode == SDL_SCANCODE_F) {
            //flag
            Uint32 FullScreenflag = SDL_GetWindowFlags(app.window);

            //si on est en plein ecran alors on retourne en fenetrer
            if (FullScreenflag & SDL_WINDOW_FULLSCREEN) {
                SDL_SetWindowFullscreen(app.window, 0); //0 -> fenetrer
            }
            //Sinon on va en fullscreen
            else {
                SDL_SetWindowFullscreen(app.window, SDL_WINDOW_FULLSCREEN);
            }
        }
        //Pour exit avec escape
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            app.StateActuel = State::Quit;
        }
    }
    //KEY UP
    if (event->type == SDL_EVENT_KEY_UP) {
        //Si on est dans notre jeu alors on peut relacher les touches de notre personnage
        if (app.StateActuel == State::Game) {
            //Relache D
            if (event->key.scancode == SDL_SCANCODE_D) {
                player->bIsMovingRight = false;
            }
            //Relache A
            if (event->key.scancode == SDL_SCANCODE_A) {
                player->bIsMovingLeft = false;
            }
        }
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            player->isCurrentlyShooting = false;
        }
    }

    return SDL_APP_CONTINUE;
}

//Appeler a chaque frame par SDL au lieu de la boucle While dans Run
SDL_AppResult
SDL_AppIterate(void *appstate) {
    return GameApp::GetInstance().RunCallBacks();
}

//Pour quitter
void
SDL_AppQuit(void *appstate, SDL_AppResult result) {
    //Destructeur
}


/*

//Main de facon traditionnel
int main(int argc, char *argv[])
{

	// instance unique du Singleton
	GameApp::GetInstance().Run();

	return 0;
}
*/
