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

//CURRENT BUG LISTS
/*
 *IF I UPGRADE A WEAPON DURING THE SPECIAL ABILITY IT WILL GO BACK TO THE OLD ONE AFTER THE END OF THE CAPACITY INSTEAD OF KEEPING THE ONE BOUGHT
 *
 *
 */









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
Uint8 r = 0, g = 0, b = 0;
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


    //GENERAL
    SDL_Texture *textureBackground = nullptr;


    //-> MENU <- Font et Texts
    TTF_Font *font = nullptr;
    TTF_Font *MenuFont = nullptr;
    TTF_Font *ShopFont = nullptr;
    TTF_TextEngine *textEngine = nullptr;
    TTF_Text *fpsText = nullptr;
    TTF_Text *MenuTitle = nullptr; //Pour rajouter un Titre
    TTF_Text *BoutonCreditsText = nullptr;
    SDL_Texture *DeerLogo = nullptr;

    //Texte special
    TTF_Font *MenuSpecialFont = nullptr;
    TTF_Text *MenuSpecialDraw = nullptr;

    //creation des boutons pour le menu
    SDL_FRect BoutonPlay = {760, 600, 400, 80};
    bool bClickedOnPlay = false;
    bool bClickedOnQuit = false;
    bool bClickedOnScore = false;
    bool bClickedOnShop = false;
    SDL_FRect BoutonScore = {785, 700, 350, 70};
    SDL_FRect BoutonShop = {810, 790, 300, 60};
    SDL_FRect BoutonQuit = {835, 870, 250, 50};
    SDL_FRect BoutonCredits = {860, 940, 200, 40};
    //Pour taille de police

    TTF_Font *StartFont = nullptr;
    TTF_Font *ScoreFont = nullptr;
    TTF_Font *QuitFont = nullptr;
    TTF_Font *CreditsFont = nullptr;
    TTF_Text *TextStart = nullptr;
    TTF_Text *TextQuit = nullptr;
    TTF_Text *TextScore = nullptr; //Pour les boutons Start Quit et TBh
    TTF_Text *TextShop = nullptr;
    TTF_Text *TextQuitReturnMenu = nullptr;


    // -> ChoixNiveau <-
    //Font titre meme que les autres
    TTF_Font *ChoixNiveauFont = nullptr;
    TTF_Text *ChoixNiveauTitre = nullptr;
    TTF_Text *ChoixNiveau1Text = nullptr;
    TTF_Text *ChoixNiveau2Text = nullptr;
    TTF_Text *ChoixNiveau3Text = nullptr;

    SDL_FRect BoutonChoixNiveau1 ={200, 550, 300,300};
    SDL_FRect BoutonChoixNiveau2 ={800, 550, 300,300};
    SDL_FRect BoutonChoixNiveau3 ={1400, 550, 300,300};


    // -> INTRONIVEAU 1 / 2 / 3 <-
    SDL_Texture *HumainTexture = nullptr;
    SDL_Texture *DeerTexture = nullptr;
    int indexMessageIntroNiveau1 = 0;
    int indexMessageIntroNiveau2 = 0;
    int indexMessageIntroNiveau3 = 0;

    static const int NB_MESSAGES_IntroNiveau1 = 3;
    const char *phrasesIntroNiveau1[NB_MESSAGES_IntroNiveau1] = {
        "Humain : We must defend againts this invasion !",
        "Deer : You will soon be under our control ",
        "Humain : Free our World ! For our freedom !"
    };
    static const int NB_MESSAGES_IntroNiveau2 = 4;
    const char *phrasesIntroNiveau2[NB_MESSAGES_IntroNiveau2] = {
        "Humain : Our defence has not been breached ",
        "Humain : We must attack them at their own territories",
        "Deer : Come!~ We are waiting for you",
        "Humain : Prepare yourself ! Humainkind is at risk"
    };
    static const int NB_MESSAGES_IntroNiveau3 = 3;
    const char *phrasesIntroNiveau3[NB_MESSAGES_IntroNiveau3] = {
        "TBD",
        "TBD",
        "TBD"
    };


    TTF_Text* texteIntroCerfEtHUmain = nullptr; // Le texte qui sera affiché


    // -> GAME <- Text et Fonts
    TTF_Font *InventoryFont = nullptr; // Game + Shop
    TTF_Text *InventoryText = nullptr; // Game + Shop
    SDL_FRect MeatInventory ={1700.0f, 10.0f, 60.0f,60.0f};
    SDL_Texture *MeatInventoryTexture = nullptr;
    SDL_Texture *ScoreUI = nullptr;
    SDL_Texture *HealUI = nullptr;
    SDL_FRect scoreSize = { 1570.0f, 925.0f, 350.0f, 140.0f };
    SDL_FRect healSize = {50.0f,1000.0f,250.0f,100.0f};
    SDL_FRect shieldSize = {150.0f, 1000, 250, 100 };
    TTF_Text *dynamicscoreText = nullptr;
    TTF_Font *dynamicscoreFont = nullptr;
    TTF_Text *dynamicPlayerHeal = nullptr;
    TTF_Font *dynamicPlayerHealFont = nullptr;
    TTF_Font *competenceSpecialFont = nullptr;
    TTF_Text *competenceSpecialText = nullptr;
    TTF_Text *competenceSpecialText2 = nullptr;// Gris de base
    //Font pour le Shield HP amount
    TTF_Font *dynamicShieldHPFont = nullptr;
    TTF_Text *dynamicShieldHPText = nullptr;
    SDL_Texture *ShieldUI = nullptr;
    //Texture fraise
    SDL_Texture* textureStrawberry = nullptr;
    //Texture Cerf
    SDL_Texture *textureCerf = nullptr;
    //Texture des differents Bullets
    SDL_Texture *textureBulletNormal = nullptr;
    SDL_Texture *textureBulletFire = nullptr;
    SDL_Texture *textureBulletIce = nullptr;
    //Texture du meat
    SDL_Texture *textureMeat = nullptr;
    //Texture boss
    SDL_Texture *textureBossStage_1_2 = nullptr;

    // -> WINSCREEN <-
    TTF_Font *WinScreenFont = nullptr;
    TTF_Font *WinScreenSousFont = nullptr;
    TTF_Text *WinScreenTitleText = nullptr;
    TTF_Text *WinScreenSousTitleText = nullptr;
    TTF_Text *WinScreenReturnMenuText = nullptr;
    SDL_FRect BoutonWinReturnMenu = {825,700,300,100};
    // -> DEATHSCREEN <-
    float deathFadeAlpha = 0.0f;
    bool bIsResetDone = false;
    TTF_Font *DeathScreenFont = nullptr;
    TTF_Font *DeathScreenSousFont = nullptr;
    TTF_Text *DeathScreenTitleText = nullptr;
    TTF_Text *DeathScreenSousTitleText = nullptr;
    TTF_Text *DeathScreenReturnMenuText = nullptr;
    SDL_FRect BoutonDeathReturnMenu = {825,700,300,100};


    // ->  PAUSE <-
    SDL_FRect BoutonResume = {850,490,300,80};
    SDL_FRect BoutonGoShop = {850, 600, 300, 80};
    SDL_FRect BoutonReturnMenu = {850,710,300,80};
    TTF_Font *FontPause = nullptr;
    TTF_Text *TextResume = nullptr;
    TTF_Text *TextPauseGoShop = nullptr;
    TTF_Text *TextReturnMenuPause = nullptr;

    // -> Score <- Text et Fonts
    TTF_Font *ReturnBoutonFont = nullptr;
    SDL_FRect BoutonQuitRetourMenu = {1600, 900, 200, 100};
    TTF_Text *ScoreMenuText = nullptr;

    // -> Shop <- Text et Fonts
    //Fonts
    TTF_Font *BoutonUpgradeFont = nullptr;
    TTF_Font *Arme_Shield_DescriptionFont = nullptr;
    TTF_Font *currentMeatShopFont = nullptr;
    //Texts
    TTF_Text *BoutonUpgradeText = nullptr;
    TTF_Text *BoutonHPUpgradeText = nullptr;
    //Text armes
    TTF_Text *ArmeFireText = nullptr;
    TTF_Text *ArmeIceText = nullptr;
    TTF_Text *ArmeTBDText = nullptr;
    //Text shields
    TTF_Text *ShieldSmallText = nullptr;
    TTF_Text *ShieldMediumText = nullptr;
    TTF_Text *ShieldLargeText = nullptr;
    //Pour faciliter le joueur a savoir si il peut en acheter un ou non une arme
    TTF_Text *statusFire = nullptr;
    TTF_Text *statusIce = nullptr;
    TTF_Text *statusTbd = nullptr;
    //Pour faciliter le joueur a savoir si il peut en acheter un ou non un shield
    TTF_Text *statusSmallShield = nullptr;
    TTF_Text *statusMediumShield = nullptr;
    TTF_Text *statusLargeShield = nullptr;

    TTF_Text *currentMeatShop = nullptr;//savoir combien de meat on a en ce moment
    TTF_Text *resumeGameShopText = nullptr;
    //rectangle
    SDL_FRect MeatInventoryShop ={180.0f, 80.0f, 100.0f,100.0f};
    SDL_FRect BoutonResumeGameShop = {1350.0f, 900.0f, 200.0f,100.0f};
    //Boutons
    SDL_FRect BoutonUpgrade = {800, 800, 125, 100};
    SDL_FRect BoutonShieldUpgrade = {1000, 800, 125, 100};

    TTF_Text *ShopMenuText = nullptr;

    // -> Credits <- Text et Fonts
    TTF_Font *Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont = nullptr;
    TTF_Font *CreditsNameFont = nullptr;
    TTF_Font *CreditsRoleFont = nullptr;
    TTF_Text *CreditsMenuText = nullptr;
    TTF_Text *CreditsName1Text = nullptr;
    TTF_Text *CreditsName2Text = nullptr;
    TTF_Text *CreditsName3Text = nullptr;
    TTF_Text *CreditsRoleText = nullptr;
    TTF_Text *CreditsRoleText2 = nullptr;
    TTF_Text *CreditsRoleText3 = nullptr;

// -> TOUCHE CLAVIER PATTERN COMMAND<-
    Player* player = nullptr;
    std::map<SDL_Scancode, Command*> keyBindings;
    std::map<SDL_Scancode, Command*> keyReleaseBindings;


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
    const Sint16 DEADZONE = 8000;          // Zone morte du gamepad
    //Boutons gbutton
    int selectedButtonMenu = 0;
    int selectedButtonChoixNiveau = 0;
    int selectedButtonScore = 0;
    int selectedButtonWin = 0;
    int selectedButtonDeath = 0;
    int selectedButtonShop = 0;
    int selectedButtonCredits = 0;
    int selectedButtonPause = 0;

    //Point Meat

    int currentMeat = 0;
    int meatGrab = 1;
    //Meat Rendu
    int lastMeat = -1;

    //Point HP
    int currentHP = 150;
    int lastHP = 1;

    //Point Shield
    int lastShield = 1;



    //Pour SHOP
    //Pour les armes
    int currentWeaponLevel = 0;
    int globalWeaponLevel = 0;
    //Pour Les shields
    int currentShieldLevel = 0;
    int globalShieldLevel = 0;

    //POUR LES FONCTIONS DES WAVES
    int currentWave = 0;
    WaveType currentWaveType; //L'enum de state
    bool waveInProgress = false;

    float survivalTimer = 0.0f;   // pour la vague survivor
    float survivalDuration = 15.0f;
    float meteorSpawnTimer = 0.0f;//La vitesse qu'une meteorite spawn

    //Pour faire attendre un peu entre chaque vagues
    bool isTransitioning = false;
    float transitionTimer = 0.0f;
    const float TRANSITION_DURATION = 2.0f; //2 seconde entre transition

    //POUR TEXTE ET FONDU DES WAVES
    TTF_Font *waveDynamicNumberFont = nullptr;
    TTF_Text *waveDynamicNumberText = nullptr;
    //BOOL POUR DIRE SI FAUT METTRE OU PAS METTRE LE UI DES WAVES
    bool showWaveUI = false;
    //Fondu Des Waves
    float waveFadeAlpha = 0.0f;



private:
    //Score Lorsque Cerf Mort
    int currentScore = 0;
    int scorePerDeerKilled = 250;


    //SCORE DU RENDER JEU
    int lastScore = -1;

    GameApp() //Constructeur
    {
        //initionalisation du GAMEPAD
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) == false) {
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
        }//Pour Le Logo
        DeerLogo = IMG_LoadTexture(renderer, "assets/Deer_Logo.png");
        if (DeerLogo == nullptr) {
            SDL_LogWarn(0, "SDL_Image failed to load DeerLogo", "assets/Deer_Logo.png", SDL_GetError());
        }
        SDL_SetTextureScaleMode(DeerLogo, SDL_SCALEMODE_NEAREST);




        ScoreUI = IMG_LoadTexture(renderer, "assets/ScoreUI.png");
        if (ScoreUI == nullptr) {
            SDL_LogWarn(0, "SDL_IMAGE FAILED TO LOAD TEXTURE ", "assets/spritesheet.png", SDL_GetError());
        }
        SDL_SetTextureScaleMode(ScoreUI, SDL_SCALEMODE_NEAREST);
        HealUI = IMG_LoadTexture(renderer, "assets/HealUI.png");
        if (HealUI == nullptr) {
            SDL_LogWarn(0, "SDL_IMAGE FAILED TO LOAD TEXTURE ", "assets/spritesheet.png", SDL_GetError());
        }
        SDL_SetTextureScaleMode(HealUI, SDL_SCALEMODE_NEAREST);



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

        ReturnBoutonFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 44);

        ShopFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 50);
        StartFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 75);
        ScoreFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 60);
        QuitFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 40);
        CreditsFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 30);

        if (ReturnBoutonFont == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        TextStart = TTF_CreateText(textEngine, StartFont, "Start", 25);
        if (MenuSpecialDraw == nullptr) {
            SDL_LogWarn(0, "Les boutons du menu n'a pas chargé : TTF", SDL_GetError());
        }
        TextQuit = TTF_CreateText(textEngine, QuitFont, "Quit", 25);
        if (TextQuit == nullptr) {
            SDL_LogWarn(0, "Les boutons du menu n'a pas chargé : TTF", SDL_GetError());
        }
        TextScore = TTF_CreateText(textEngine, ScoreFont, "Score", 25);
        if (TextScore == nullptr) {
            SDL_LogWarn(0, "Les boutons du menu n'a pas chargé : TTF", SDL_GetError());
        }
        TextShop = TTF_CreateText(textEngine, ShopFont, "Shop", 25);
        if (TextShop == nullptr) {
            SDL_LogWarn(0, "Le text du bouton Shop n'a pas fonctionner", SDL_GetError());
        }

        BoutonUpgradeFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 25);
        BoutonUpgradeText = TTF_CreateText(textEngine, BoutonUpgradeFont, " Weapon \nUpgrade", 25);
        if (TTF_SetTextColor(BoutonUpgradeText, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "Couleur du bouton amelioration n'a pas fonctionné", SDL_GetError());
        }
        BoutonHPUpgradeText = TTF_CreateText(textEngine, BoutonUpgradeFont, "   Shield  \nUpgrade", 25);
        if (TTF_SetTextColor(BoutonHPUpgradeText, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "Couleur de Bouton HP Upgrade", SDL_GetError());
        }
        BoutonCreditsText = TTF_CreateText(textEngine, CreditsFont, "Credits", 25);

        if (TTF_SetTextColor(TextShop, 150, 100, 255, 255) == false) {
            SDL_LogWarn(0, "La couleur de TextShop n'a pas fonctionné", SDL_GetError());
        }
        if (TTF_SetTextColor(TextStart, 0,  255, 255, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        if (TTF_SetTextColor(TextQuit, 255, 80, 255, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        if (TTF_SetTextColor(TextScore, 80, 150, 255, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
        }
        if (TTF_SetTextColor(BoutonCreditsText, 200,150,200,255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());

        }
        TextQuitReturnMenu = TTF_CreateText(textEngine, ReturnBoutonFont, "Return \nMenu", 25);
        if (TextQuitReturnMenu == nullptr) {
            SDL_LogWarn(0, "SDL_TTF failed to set the return Menu text)! %s", SDL_GetError());
        }
        if (TTF_SetTextColor(TextQuitReturnMenu, 0, 0, 0, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set color TextQuitScore %s", SDL_GetError());
        }
        //FONT POUR TITRE SCORE, SHOP, CREDITS, DeathScreen
        Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 108);
        // CHOIX NIVEAU
        ChoixNiveauFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 30);
        if (ChoixNiveauFont == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to set the font", SDL_GetError());
        }
        ChoixNiveauTitre = TTF_CreateText(textEngine, Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont, "Chose Your Level", 25);
        ChoixNiveau1Text = TTF_CreateText(textEngine, ChoixNiveauFont, "Defend The Sky", 25);
        ChoixNiveau2Text = TTF_CreateText(textEngine, ChoixNiveauFont, "Work In Progress", 25);
        ChoixNiveau3Text = TTF_CreateText(textEngine, ChoixNiveauFont, "TBD", 25);


        //DANS INTROGAME

        texteIntroCerfEtHUmain = TTF_CreateText(textEngine, ShopFont, phrasesIntroNiveau1[0], 0);
        TTF_SetTextColor(texteIntroCerfEtHUmain, 255, 255, 255, 255);


        //DANS GAME
        dynamicscoreFont = TTF_OpenFont("assets/font.ttf", 40);
        dynamicscoreText = TTF_CreateText(textEngine, dynamicscoreFont, "Score", 25);
        if (dynamicscoreText == nullptr) {
            SDL_LogWarn(0, "failed to set the text of the dynamicscoreText", SDL_GetError());
        }
        //r g b mis dans la fonction Game

        InventoryFont = TTF_OpenFont("assets/font.ttf", 40);
        InventoryText = TTF_CreateText(textEngine, InventoryFont, " 0 ", 25);
        if (InventoryText == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to set the inventory text", SDL_GetError());
        }
        dynamicPlayerHealFont = TTF_OpenFont("assets/font.ttf", 25);
        dynamicPlayerHeal = TTF_CreateText(textEngine, dynamicPlayerHealFont, "Heal: 150", 25);
        if (dynamicPlayerHeal == nullptr) {
            SDL_LogWarn(0,"failed to create the text of dynamicPlayerHeal", SDL_GetError());
        }
        if (TTF_SetTextColor(dynamicPlayerHeal, 255,255,255,255) == false) {
            SDL_LogWarn (1, "failed to make the color of dynamicPlayerHeal", SDL_GetError());
        }
        competenceSpecialFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 20);
        competenceSpecialText = TTF_CreateText(textEngine, competenceSpecialFont, "special Skill Available", 25);
        if (competenceSpecialText == nullptr) {
            SDL_LogWarn(0, "failed to create the text for competenceSpecialText", SDL_GetError());
        }
        if (TTF_SetTextColor(competenceSpecialText, 80, 80, 220, 255) == false) {
            SDL_LogWarn(0, "failed to set the color of competenceSpecialText", SDL_GetError());
        }
        competenceSpecialText2 = TTF_CreateText(textEngine, competenceSpecialFont, "special Skill Available", 25);
        if (competenceSpecialText2 == nullptr) {
            SDL_LogWarn(0, "failed to create the text for competenceSpecialText", SDL_GetError());
        }
        dynamicShieldHPFont = TTF_OpenFont("assets/font.ttf", 25);
        dynamicShieldHPText = TTF_CreateText(textEngine, dynamicShieldHPFont, "Shield Amount: 0", 25);
        if (dynamicShieldHPText == nullptr) {
            SDL_LogWarn(0, "failed to set the text of dynamicShieldHPText", 25, SDL_GetError());
        }
        if (TTF_SetTextColor(dynamicShieldHPText, 255, 255, 255, 255) == false) {
            SDL_LogWarn(0, "failed to set the color of dynamicShieldHPText");
        }
        //Pour faire spawn la fraise
        textureStrawberry = IMG_LoadTexture(renderer, "assets/Strawb.png");
        if (textureStrawberry == nullptr) {
            SDL_LogWarn(0, "Erreur chargement Strawb.png: %s", SDL_GetError());
        }
        //Pour faire spawn la texture du Cerf
        textureCerf = IMG_LoadTexture(renderer, "assets/DeerEnnemie.png");
        if (textureCerf == nullptr) {
            SDL_LogWarn(0, "failed to set the texture of textureCerf", SDL_GetError());
        }
        //Pour les textures des differents bullets
        textureBulletNormal = IMG_LoadTexture(renderer, "assets/BulletNormal.png");
        if (textureBulletNormal == nullptr) {
            SDL_LogWarn(0, "failed to set the texture of textureBulletNormal", SDL_GetError());
        }
        textureBulletFire = IMG_LoadTexture(renderer, "assets/BulletFire.png");
        if (textureBulletFire == nullptr) {
            SDL_LogWarn(0, "failed to set the texture of textureBulletFire", SDL_GetError());
        }
        textureBulletIce = IMG_LoadTexture(renderer, "assets/BulletIce.png");
        if (textureBulletIce == nullptr) {
            SDL_LogWarn(0, "failed to set the texture of textureBulletIce", SDL_GetError());
        }
        //MEAT TEXTURE
        textureMeat = IMG_LoadTexture(renderer, "assets/Meatv3.png");
        if (textureMeat == nullptr) {
            SDL_LogWarn(0, "failed to set the texture of textureMeat", SDL_GetError());
        }
        //BACKGROUND TEXTURE
        textureBackground = IMG_LoadTexture(renderer, "assets/Background.png");
        if (textureBackground == nullptr) {
            SDL_LogWarn(0, "failed to set the texture of textureBackground", SDL_GetError());
        }
        //BOSS STAGE 1 AND 2
        textureBossStage_1_2 = IMG_LoadTexture(renderer, "assets/BossFinalDeerInvaders.png");
        if (textureBossStage_1_2 == nullptr) {
            SDL_LogWarn(0,"failed to load textureBossStage_1_2.png", SDL_GetError());
        }


        //POUR PAUSE
        FontPause = TTF_OpenFont("assets/Cosmo Corner.ttf", 40);
        if (FontPause == nullptr) {
            SDL_LogWarn(0, "failed to set the font: FontPause ", SDL_GetError());
        }
        TextResume = TTF_CreateText(textEngine, FontPause, "Resume", 25);
        if (TextResume == nullptr) {
            SDL_LogWarn(0, "failed to set the text: TextResume", SDL_GetError());
        }
        if (TTF_SetTextColor(TextResume, 173, 216, 230, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set the color of: TextResume", SDL_GetError());
        }
        TextPauseGoShop = TTF_CreateText(textEngine, FontPause, "Shop", 25);
        if (TextPauseGoShop == nullptr) {
            SDL_LogWarn(0, "failed to set the text: TextResume", SDL_GetError());
        }
        if (TTF_SetTextColor(TextPauseGoShop, 0, 191, 255, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set the color of: TextResume", SDL_GetError());
        }
        TextReturnMenuPause = TTF_CreateText(textEngine, FontPause, "Return Menu", 25);
        if (TextReturnMenuPause == nullptr) {
            SDL_LogWarn(0, "SDL_ttf failed to set TextReturnMenuPause text", SDL_GetError());
        }
        if (TTF_SetTextColor(TextReturnMenuPause, 65, 105, 255, 255 ) == false) {
            SDL_LogWarn(0, "failed to set the color of: TextReturnMenuPause ", SDL_GetError());
        }
        //FONT POUR LES WAVES
        waveDynamicNumberFont = TTF_OpenFont("assets/font.ttf", 70);
        waveDynamicNumberText = TTF_CreateText(textEngine, waveDynamicNumberFont, "Wave 1", 25);
        if (waveDynamicNumberText == nullptr) {
            SDL_LogWarn(0,"Erreur failed to set waveDynamicNumberText", SDL_GetError());
        }
        if (TTF_SetTextColor(waveDynamicNumberText, 255, 255, 255, 255) == false) {
            SDL_LogWarn(1,"Failed to set the color for waveDynamicNumberText", SDL_GetError());
        }

        //Score
        ScoreMenuText = TTF_CreateText(textEngine,Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont, "Score", 25);
        if (ScoreMenuText == nullptr) {
            SDL_LogWarn(0,"failed to set the text of Shop title", SDL_GetError());
        }
        if (TTF_SetTextColor(ScoreMenuText, 255,255,255,255) == false) {
            SDL_LogWarn(0,"Erreur couleur du titre Score n'a pas changer");
        }
        //WINSCREEN
        WinScreenFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 40);
        WinScreenSousFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 40);
        WinScreenTitleText = TTF_CreateText(textEngine,Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont, "YOU WON", 25);//font du titre
        if (WinScreenTitleText == nullptr) {
            SDL_LogWarn(0,"failed to set the text of WinScreenTitleText", SDL_GetError());
        }
        if (TTF_SetTextColor(WinScreenTitleText, 255,255,255,255) == false) {
            SDL_LogWarn(1, "failed to set the color of DeathScreenTitleText");
        }
        WinScreenSousTitleText = TTF_CreateText(textEngine,WinScreenSousFont, "You finished this stage", 25);
        if (WinScreenSousTitleText == nullptr) {
            SDL_LogWarn(0, "failed to set the text of WinScreenSousTitleText");
        }
        if (TTF_SetTextColor(WinScreenSousTitleText , 255,255,255,255) == false) {
            SDL_LogWarn(0, "failed to set the color of WinScreenSousTitleText");
        }
        WinScreenReturnMenuText = TTF_CreateText(textEngine, WinScreenFont, "Return Menu", 25);//Pour le bouton
        if (WinScreenReturnMenuText == nullptr) {
            SDL_LogWarn(0, "failed to set the text of WinScreenReturnMenuText", SDL_GetError());
        }
        if (TTF_SetTextColor(WinScreenReturnMenuText, 0,0,0,255) == false) {
            SDL_LogWarn(0, "failed to set the color of WinScreenReturnMenuText");
        }

        //DEATHSCREEN
        DeathScreenFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 40);
        DeathScreenSousFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 60);
        DeathScreenTitleText = TTF_CreateText(textEngine,Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont, "GAME OVER", 25);//font du titre
        if (DeathScreenTitleText == nullptr) {
            SDL_LogWarn(0,"failed to set the text of DeathScreenTitleText", SDL_GetError());
        }
        if (TTF_SetTextColor(DeathScreenTitleText, 255,255,255,255) == false) {
            SDL_LogWarn(1, "failed to set the color of DeathScreenTitleText");
        }
        DeathScreenSousTitleText = TTF_CreateText(textEngine,DeathScreenSousFont, "You died", 25);
        if (DeathScreenSousTitleText == nullptr) {
            SDL_LogWarn(0, "failed to set the text of DeathScreenSousTitleText");
        }
        if (TTF_SetTextColor(DeathScreenSousTitleText , 255,255,255,255) == false) {
            SDL_LogWarn(0, "failed to set the color of DeathScreenSousTitleText");
        }
        DeathScreenReturnMenuText = TTF_CreateText(textEngine, DeathScreenFont, "Return Menu", 25);//Pour le bouton
        if (DeathScreenReturnMenuText == nullptr) {
            SDL_LogWarn(0, "failed to set the text of DeathScreenReturnMenuText", SDL_GetError());
        }
        if (TTF_SetTextColor(DeathScreenReturnMenuText, 0,0,0,255) == false) {
           SDL_LogWarn(0, "failed to set the color of DeathScreenReturnMenuText");
        }

        //SHOP
        ShopMenuText = TTF_CreateText(textEngine, Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont, "Shop", 25);
        if (ShopMenuText == nullptr) {
            SDL_LogWarn(0,"failed to set the text of Shop title", SDL_GetError());
        }
        if (TTF_SetTextColor(ShopMenuText, 255, 255, 255, 255) == false) {
            SDL_LogWarn(2,"Erreur de couleur pour le titre de Shop -> Shop", SDL_GetError());
        }
        Arme_Shield_DescriptionFont = TTF_OpenFont("assets/font.ttf", 40);
        if (Arme_Shield_DescriptionFont == nullptr) {
            SDL_LogWarn(0,"failed to implement the font Arme_Shield_DescriptionFont", SDL_GetError());
        }
        ArmeFireText = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "Weapon Fire - 10 Meats -", 25);
        if (ArmeFireText == nullptr) {
            SDL_LogWarn(1, "failed to set the ArmeFireText ", SDL_GetError());
        }
        if (TTF_SetTextColor(ArmeFireText, 247, 65, 27, 255) == false) {
            SDL_LogWarn(2,"failed to put the color of ArmeFireText",SDL_GetError());
        }
        ArmeIceText = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "Weapon Ice - 50 Meats -", 25);
        if (ArmeIceText == nullptr) {
            SDL_LogWarn(1, "failed to set the ArmeIceText ", SDL_GetError());
        }
        if (TTF_SetTextColor(ArmeIceText, 173, 216, 216, 230) == false) {
            SDL_LogWarn(2,"failed to put the color of ArmeIceText",SDL_GetError());
        }
        ArmeTBDText = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "Arme TBD - 100 Meats -", 25);
        if (ArmeTBDText == nullptr) {
            SDL_LogWarn(1, "failed to set the ArmeFireText ", SDL_GetError());
        }
        if (TTF_SetTextColor(ArmeTBDText, 255, 255, 255, 255) == false) {
            SDL_LogWarn(2,"failed to put the color of ArmeTBDText",SDL_GetError());
        }//Status des armes
        statusFire = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "LEVEL 2", 25);
        if (statusFire == nullptr) {
            SDL_LogWarn(1, "failed to set the ArmeFireText ", SDL_GetError());
        }
        statusIce = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "LEVEL 3", 25);
        if (statusIce == nullptr) {
            SDL_LogWarn(1, "failed to set the ArmeFireText ", SDL_GetError());
        }
        statusTbd = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "LEVEL 4", 25);
        if (statusTbd == nullptr) {
            SDL_LogWarn(1, "failed to set the ArmeFireText ", SDL_GetError());
        }

        //SHIELD TTF
        ShieldSmallText = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "Small Shield - 10 Meats -", 25);
        if (ShieldSmallText == nullptr) {
            SDL_LogWarn(1, "failed to set the ShieldSmallText ", SDL_GetError());
        }
        if (TTF_SetTextColor(ShieldSmallText, 85,23,255,255) == false) {
            SDL_LogWarn(2,"failed to put the color of ShieldSmallText",SDL_GetError());
        }
        ShieldMediumText = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "Medium Shield - 50 Meats -", 25);
        if (ShieldMediumText == nullptr) {
            SDL_LogWarn(1, "failed to set the ShieldMediumText ", SDL_GetError());
        }
        if (TTF_SetTextColor(ShieldMediumText, 186, 23, 255, 255) == false) {
            SDL_LogWarn(2,"failed to put the color of ShieldMediumText",SDL_GetError());
        }
        ShieldLargeText = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "Large Shield - 100 Meats -", 25);
        if (ShieldLargeText == nullptr) {
            SDL_LogWarn(1, "failed to set the ShieldLargeText ", SDL_GetError());
        }
        if (TTF_SetTextColor(ShieldLargeText, 233,255,23,255) == false) {
            SDL_LogWarn(2,"failed to put the color of ShieldLargeText",SDL_GetError());
        }//Status des armes
        statusSmallShield = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "LEVEL 2", 25);
        if (statusSmallShield == nullptr) {
            SDL_LogWarn(1, "failed to set the statusSmallShield ", SDL_GetError());
        }
        statusMediumShield = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "LEVEL 3", 25);
        if (statusMediumShield == nullptr) {
            SDL_LogWarn(1, "failed to set the statusMediumShield ", SDL_GetError());
        }
        statusLargeShield = TTF_CreateText(textEngine, Arme_Shield_DescriptionFont, "LEVEL 4", 25);
        if (statusLargeShield == nullptr) {
            SDL_LogWarn(1, "failed to set the statusLargeShield ", SDL_GetError());
        }




        currentMeatShopFont = TTF_OpenFont("assets/font.ttf", 60);
        currentMeatShop = TTF_CreateText(textEngine, currentMeatShopFont, "0", 25);
        if (currentMeatShop == nullptr) {
            SDL_LogWarn(1, "failed to set the currentMeatShop", SDL_GetError());
        }
        if (TTF_SetTextColor(currentMeatShop, 255, 255, 255, 255) == false) {
            SDL_LogWarn(1, "failed to set the color of currentMeatShop", SDL_GetError());
        }
        resumeGameShopText = TTF_CreateText(textEngine, ReturnBoutonFont, "Resume \nGame",25);
        if (resumeGameShopText == nullptr) {
            SDL_LogWarn(1, "failed to initialise text resumeGameShopText", SDL_GetError());
        }
        if (TTF_SetTextColor(resumeGameShopText, 0,0,0,255) == false) {
            SDL_LogWarn(1,"Erreur impossible de mettre la couleur de resueGameShopText",SDL_GetError());
        }



        //credits
        CreditsRoleFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 50);//Pour les roles
        CreditsNameFont = TTF_OpenFont ("assets/Cosmo Corner.ttf", 40);//Pour les noms
        Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont = TTF_OpenFont("assets/Cosmo Corner.ttf", 108);
        CreditsMenuText = TTF_CreateText(textEngine, Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont, "Credits", 25);
        if (CreditsMenuText == nullptr) {
            SDL_LogWarn(0, "EERREUR ! Le texte CreditsText n'a pas charger", SDL_GetError());
        }
        if (TTF_SetTextColor(CreditsMenuText, 255, 255, 255, 255) == false) {
            SDL_LogWarn(0, "SDL_ttf failed to set color TextQuitScore %s", SDL_GetError());
        }
        CreditsName1Text = TTF_CreateText(textEngine, CreditsNameFont, "Louis ", 25);
        if (CreditsName1Text == nullptr) {
            SDL_LogWarn(0,"Impossible de creer le nom", SDL_GetError());
        }
        if (TTF_SetTextColor(CreditsName1Text, 255,255,255,255) == false) {
            SDL_LogWarn(0,"Couleur pour Nom n'a pas fonctionner");
        }
        CreditsName2Text = TTF_CreateText(textEngine, CreditsNameFont,"Nom", 25);
        if (CreditsName2Text == nullptr){
            SDL_LogWarn(0,"Impossible de charger le text Nom2", SDL_GetError());
        }
        if (TTF_SetTextColor(CreditsName2Text, 255, 255, 255,255) == false) {
            SDL_LogWarn(0, "Couleur non fonctionnel", SDL_GetError());
        }
        CreditsName3Text = TTF_CreateText(textEngine, CreditsNameFont,"Nom", 25);
        if (CreditsName3Text == nullptr){
            SDL_LogWarn(0,"Impossible de charger le text Nom2", SDL_GetError());
        }
        if (TTF_SetTextColor(CreditsName3Text, 255, 255, 255,255) == false) {
            SDL_LogWarn(0, "Couleur non fonctionnel", SDL_GetError());
        }
        CreditsRoleText = TTF_CreateText(textEngine, CreditsRoleFont, "Role Patate : ", 25);
        if (CreditsRoleText == nullptr) {
            SDL_LogWarn(0, "Impossible de changer le role", SDL_GetError());
        }
        if (TTF_SetTextColor(CreditsRoleText, 255,255,255,255) == false) {
            SDL_LogWarn(0,"Erreur couleur Role1", SDL_GetError());
        }
        CreditsRoleText2 = TTF_CreateText(textEngine, CreditsRoleFont, "Role Pomme : ", 25);
        if (CreditsRoleText2 == nullptr) {
            SDL_LogWarn(0, "Impossible de changer le role", SDL_GetError());
        }
        if (TTF_SetTextColor(CreditsRoleText2, 255,255,255,255) == false) {
            SDL_LogWarn(0,"Erreur couleur Role1", SDL_GetError());
        }
        CreditsRoleText3 = TTF_CreateText(textEngine, CreditsRoleFont, "Role Fraise : ", 25);
        if (CreditsRoleText3 == nullptr) {
            SDL_LogWarn(0, "Impossible de changer le role", SDL_GetError());
        }
        if (TTF_SetTextColor(CreditsRoleText3, 255,255,255,255) == false) {
            SDL_LogWarn(0,"Erreur couleur Role1", SDL_GetError());
        }


        // -> Dans Game <-

        //Joueur
        player = new Player();
        // Assigne les textures aux types d'armes
        player->currentWeapon->texture = textureBulletNormal;
        entities.push_back(player);

        //Timer FPS
        fpsTimerID = SDL_AddTimer(250, TimerCallback, &shouldUpdateText);
    }

    //Libere memoire
    ~GameApp() {
        SDL_RemoveTimer(fpsTimerID);
        TTF_DestroyText(fpsText);
        //-> Choix Niveau <-
        TTF_DestroyText(ChoixNiveau1Text);
        TTF_DestroyText(ChoixNiveau2Text);
        TTF_DestroyText(ChoixNiveau3Text);
        TTF_CloseFont(ChoixNiveauFont);

        //Shop
        TTF_CloseFont (Arme_Shield_DescriptionFont);
        TTF_DestroyText(ShieldSmallText);
        TTF_DestroyText(ShieldMediumText);
        TTF_DestroyText(ShieldLargeText);
        TTF_DestroyText (ArmeFireText);
        TTF_DestroyText (ArmeIceText);
        TTF_DestroyText (ArmeTBDText);


        TTF_DestroyText(MenuTitle);
        TTF_DestroyRendererTextEngine(textEngine);
        TTF_CloseFont(font);
        TTF_CloseFont(ShopFont);
        TTF_CloseFont(StartFont);
        TTF_CloseFont(QuitFont);
        TTF_CloseFont(ScoreFont);
        TTF_CloseFont(waveDynamicNumberFont);
        TTF_CloseFont(CreditsFont);
        TTF_CloseFont(Credits_Shop_Score_WinScreen_DeathScreen_ChoixNiveau_TitleFont);
        TTF_CloseFont(FontPause);
        TTF_DestroyText(TextStart);
        TTF_DestroyText(TextQuit);
        TTF_DestroyText(TextScore);
        TTF_DestroyText(CreditsMenuText);
        TTF_DestroyText(waveDynamicNumberText);
        TTF_DestroyText(TextResume);
        TTF_DestroyText(TextReturnMenuPause);
        TTF_CloseFont(MenuSpecialFont);
        SDL_DestroyTexture(spritesheet);
        SDL_DestroyTexture(DeerLogo);
        SDL_DestroyTexture(textureStrawberry);
        SDL_DestroyTexture(textureCerf);
        SDL_DestroyTexture(ScoreUI);
        SDL_DestroyTexture(HealUI);
        SDL_DestroyTexture(textureBulletNormal);
        SDL_DestroyTexture(textureBulletFire);
        SDL_DestroyTexture(textureBulletIce);
        SDL_DestroyTexture(textureMeat);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        if (gameController) {
            SDL_CloseGamepad(gameController);
        }
        for (auto& ent : entities)
        {
            delete ent;
        }
        entities.clear();
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
        }//Pour le Logo Du jeu
        if (DeerLogo != nullptr) {
            float imgW, imgH;
            SDL_GetTextureSize(DeerLogo, &imgW, &imgH);
            //Calcul de sa largeur
            float targetWidth = 700.0f;
            // calcul sa hauteur proportionellement
            float targetHeight = targetWidth * (imgH / imgW);
            //Position
            SDL_FRect dst = {
                (1920.0f / 2.0f) - (targetWidth / 2.0f),
                50.0f,
                targetWidth,
                targetHeight
            };

            // On dessine avec nullptr pour prendre tout le logo Deer Invaders
            SDL_RenderTexture(renderer, DeerLogo, nullptr, &dst);
        }
    }


    //Petite fonction pour mettre un titre MENU
    void RenderTitle() {
       // TTF_DrawRendererText(MenuTitle, 0, 200);
        TTF_DrawRendererText(MenuSpecialDraw, 000, 980);
    }



    //Boutons
    void RenderBoutons(const SDL_FRect &rect, TTF_Text *buttonText, Uint8 buttonr, Uint8 buttong, Uint8 buttonb) {
        SDL_SetRenderDrawColor(renderer, buttonr, buttong, buttonb, 255);
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

    void RenderScoreTitle() {
        TTF_DrawRendererText(ScoreMenuText, 800, 150);
    }
    void RenderShopTitle() {
        TTF_DrawRendererText(ShopMenuText, 850, 150);
    }
    void RenderCreditsTitle() {
        TTF_DrawRendererText(CreditsMenuText, 800, 150);
    }
    //Rendu du titre WinTitle
    void RenderWinTitle() {
        SDL_FRect WinScreenTitleRect = {610,125,700,225};
        SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255); // vert pour la fin
        SDL_RenderFillRect(renderer, &WinScreenTitleRect);
        TTF_DrawRendererText(WinScreenTitleText, 700, 150);
        TTF_DrawRendererText(WinScreenSousTitleText, 710, 260);
    }

    //Render Titre Choix Niveau
    void RenderChoixNiveauTitle() {
        TTF_DrawRendererText(ChoixNiveauTitre, 450,150);
    }

    //Rendu Du titre DeathTitle
    void RenderDeathTitle() {
        SDL_FRect DeathScreenTitleRect = {610,125,700,225};
        SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255); // ROUGE vif
        SDL_RenderFillRect(renderer, &DeathScreenTitleRect);
        TTF_DrawRendererText(DeathScreenTitleText, 650, 150);
        TTF_DrawRendererText(DeathScreenSousTitleText, 825, 250);
    }

    //La fonction du dessin de la barre de progression des armes a amiliorer
    void RenderGlobalWeaponProgresBar(float startX, float startY) {
        //pour un petit rectangle
        int segmentWidth = 110;
        int segmentHeight = 30;
        int espace = 12;

        //3 armes
        for (int i = 0; i< 3; i++) {
            SDL_FRect segment = {startX, startY - (i * (segmentHeight + espace)), (float)segmentWidth, (float)segmentHeight};

            if (i < globalWeaponLevel) {
                // Segment rempli
                if (i == 0) SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); // Orange pour Fire
                if (i == 1) SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255); // Bleu pour Ice
                if (i == 2) SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Gris pour TBD
            } else {
                // Segment vide
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            }
            SDL_RenderFillRect(renderer, &segment);
        }

    }
    //La fonction du dessin de la barre de progression des Shields
    void RenderGlobalShieldProgresBar(float startX, float startY) {
        //pour un petit rectangle
        int segmentWidth = 110;
        int segmentHeight = 30;
        int espace = 12;

        //3 Shield
        for (int i = 0; i< 3; i++) {
            SDL_FRect segment = {startX, startY - (i * (segmentHeight + espace)), (float)segmentWidth, (float)segmentHeight};

            if (i < globalShieldLevel) {
                // Segment rempli
                if (i == 0) SDL_SetRenderDrawColor(renderer, 85,23,255,255); // Bleu (Petit Shield)
                if (i == 1) SDL_SetRenderDrawColor(renderer, 186, 23, 255, 255); // Mauve (Moyen Shield)
                if (i == 2) SDL_SetRenderDrawColor(renderer, 233,255,23,255); // Jaune (Large Shield)
            } else {
                // Segment vide
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            }
            SDL_RenderFillRect(renderer, &segment);
        }

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

        // Dessin des boutons + Selection avec manette
        //Bouton Play
        if (selectedButtonMenu == 0) {
            RenderBoutons(BoutonPlay, TextStart, r, g, b);//Couleur si selectionner
        }else {
            RenderBoutons(BoutonPlay, TextStart, 20, 20, 20);//Blanc de base
        }
        //BOUTON SCORE
        if (selectedButtonMenu == 1) {
            RenderBoutons(BoutonScore, TextScore, r, g, b);
        }else {
            RenderBoutons(BoutonScore, TextScore, 20, 20, 20);//blanc de base
        }
        //BOUTON QUIT
        if (selectedButtonMenu == 2) {
            RenderBoutons(BoutonShop, TextShop, r, g, b);
        }else {
            RenderBoutons(BoutonShop, TextShop, 20, 20, 20);
        }
        //BOUTON SHOP
        if (selectedButtonMenu == 3) {
            RenderBoutons(BoutonQuit, TextQuit, r, g, b);
        }
        else {
            RenderBoutons(BoutonQuit, TextQuit, 20, 20, 20);
        }
        //BOUTON CREDITS
        if (selectedButtonMenu == 4) {
            RenderBoutons(BoutonCredits, BoutonCreditsText, r, g, b);
        }
        else {
            RenderBoutons(BoutonCredits, BoutonCreditsText, 20, 20, 20);
        }

        TTF_DrawRendererText(fpsText, 1800, 10);
        SDL_RenderPresent(renderer);
    }


    //fonction pour le choix de niveau
    // Gros cube a cliquer pour aller vers le  niveau choix entre 3 niveau
    void ChoixNiveau(float deltaTime) {

        UpdateBackgroundTint(deltaTime);  // <- Pour le rgb
        // Rendu du menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //boutons et text
        //Bouton Niveau 1
        if (selectedButtonChoixNiveau== 0) {
            RenderBoutons(BoutonChoixNiveau1, nullptr, r, g, b);//Couleur si selectionner
        }else {
            RenderBoutons(BoutonChoixNiveau1, nullptr, 40, 40, 40);//gris foncer de base
        }
        //BOUTON Niveau 2
        if (selectedButtonChoixNiveau == 1) {
            RenderBoutons(BoutonChoixNiveau2, nullptr, r, g, b);
        }else {
            RenderBoutons(BoutonChoixNiveau2, nullptr, 40, 40, 40);//gris foncer de base
        }
        //BOUTON Niveau 3
        if (selectedButtonChoixNiveau == 2) {
            RenderBoutons(BoutonChoixNiveau3, nullptr, r, g, b);
        }else {
            RenderBoutons(BoutonChoixNiveau3, nullptr, 40, 40, 40);//gris foncer de base
        }
        TTF_DrawRendererText(ChoixNiveau1Text, 200, 900);
        TTF_DrawRendererText(ChoixNiveau2Text, 800, 900);
        TTF_DrawRendererText(ChoixNiveau3Text, 1400, 900);
        RenderChoixNiveauTitle();
        TTF_DrawRendererText(fpsText, 1800, 10);
        SDL_RenderPresent(renderer);
    }

    //fonction Intro pour la narration de debut

    void IntroGame (float deltaTime) {
        // Effacer l'écran (Fond noir par défaut)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Personnage Humain
        SDL_FRect HumainRect = {100, 300, 400, 800}; // Ajusté X pour pas qu'ils se collent
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &HumainRect);

        // Personnage Deer
        SDL_FRect DeerRect = {1400, 300, 400, 800}; // Ajusté X à droite
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour le cerf
        SDL_RenderFillRect(renderer, &DeerRect);

        // Dessiner la bande gris foncé en bas
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 200);
        //la partie basse dessiner
        SDL_FRect bottomBand = {0, 880, 1920, 200};
        SDL_RenderFillRect(renderer, &bottomBand);

        //Text dessus la bande
        TTF_DrawRendererText(fpsText, 1800, 10);
        if (texteIntroCerfEtHUmain != nullptr) {
            int weight, height;
            TTF_GetTextSize(texteIntroCerfEtHUmain, &weight, &height);

            // Centrage du texte par rapport au
            float textX = (1920 - weight) / 2.0f;
            float textY = 840 + (280 - height) / 2.0f;

            TTF_DrawRendererText(texteIntroCerfEtHUmain, textX, textY);
        }
        TTF_DrawRendererText(fpsText, 1800, 10);
        //Tout afficher
        SDL_RenderPresent(renderer);
    }
    //Pour la premiere vague d'ennemies
    void SpawnWave1() {
        entities.push_back(new Enemy_Deer(100.f, 50.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(250.f, 50.0f, true, textureCerf));
        entities.push_back(new Enemy_Deer(400.f, 50.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(550.0f, 50.0f, true, textureCerf));

        entities.push_back(new Enemy_Deer(700.0f, 50.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(850.0f, 50.0f, true, textureCerf));
        entities.push_back(new Enemy_Deer(1000.0f, 50.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(1150.0f, 50.0f,true, textureCerf));

        //Ligne 2 de cerfs
        entities.push_back(new Enemy_Deer(100.f, 180.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(250.f, 180.0f, true, textureCerf));
        entities.push_back(new Enemy_Deer(400.f, 180.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(550.0f, 180.0f, true, textureCerf));

        entities.push_back(new Enemy_Deer(700.0f, 180.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(850.0f, 180.0f,true, textureCerf));
        entities.push_back(new Enemy_Deer(1000.0f, 180.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(1150.0f, 180.0f, true, textureCerf));
        //Ligne 3 de cerfs
        entities.push_back(new Enemy_Deer(100.f, 330.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(250.f, 330.0f, true, textureCerf));
        entities.push_back(new Enemy_Deer(400.f, 330.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(550.0f, 330.0f, true, textureCerf));

        entities.push_back(new Enemy_Deer(700.0f, 330.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(850.0f, 330.0f,true, textureCerf));
        entities.push_back(new Enemy_Deer(1000.0f, 330.0f,false, textureCerf));
        entities.push_back(new Enemy_Deer(1150.0f, 330.0f,true, textureCerf));
        //Ligne 4 de cerfs
        entities.push_back(new Enemy_Deer(100.f, 480.0f,false, textureCerf));
        entities.push_back(new Enemy_Deer(250.f, 480.0f,true, textureCerf));
        entities.push_back(new Enemy_Deer(400.f, 480.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(550.0f, 480.0f,true, textureCerf));

        entities.push_back(new Enemy_Deer(700.0f, 480.0f,false, textureCerf));
        entities.push_back(new Enemy_Deer(850.0f, 480.0f,true, textureCerf));
        entities.push_back(new Enemy_Deer(1000.0f, 480.0f, false, textureCerf));
        entities.push_back(new Enemy_Deer(1150.0f, 480.0f, true, textureCerf));
    }

    //WAVE 2
    //Randomizer pour creer les meteorites dans la fonction Game()
    void SpawnWave2(float deltaTime) {
        survivalTimer += deltaTime;

        if (survivalTimer < survivalDuration) {
            meteorSpawnTimer += deltaTime;
            if (meteorSpawnTimer >= 0.7f) {
                float spawnX = static_cast<float>(rand() % 1800);
                entities.push_back(new Enemy_Meteor(spawnX, -80.0f));
                meteorSpawnTimer = 0.0f;
            }
        } else {
            bool anyMeteorLeft = false;
            for (auto& ent : entities) {
                if (ent->entityType == EntityType::Enemy && !ent->bIsDestroyed) {
                    anyMeteorLeft = true;
                    break;
                }
            }
            if (!anyMeteorLeft) {
                PreparationNextWave();
            }
        }

    }

    //Wave 3
    void SpawnWave3() {
        SDL_FPoint planetCenter = {960.0f, 300.0f};
        float radius = 550.0f;//radius auquel les cerfs tournent autour de la planet
        int nbCerfs = 32;

        for (int i = 0; i < nbCerfs; i++) {
            float angle = (2.0f * M_PI / nbCerfs) * i; // angle pi

            // Vitesse légèrement différente pour chaque cerf
            // valeur qui cycle entre 0, 1, 2, 3.
            float vitesseX = 0.8f + (static_cast<float>(i % 4) * 0.1f);
            float vitesseY = 0.3f + (static_cast<float>(i % 3) * 0.05f);

            // Chaque cerf a sa propre vitesse }} Radius similaire
            auto moveCircle = [planetCenter, radius, vitesseX, vitesseY](Enemy_Deer* cerf, float dt) {
                cerf->timeAlive += dt;
                cerf->transform.position.x = planetCenter.x + std::cos(cerf->timeAlive * vitesseX + cerf->orbitAngle) * radius;
                cerf->transform.position.y = planetCenter.y + std::sin(cerf->timeAlive * vitesseY + cerf->orbitAngle) * (radius * 0.4f);

                //cos → controle gauche/droite
                //sin → controle haut/bas
            };

            float startX = planetCenter.x + std::cos(angle) * radius;
            float startY = planetCenter.y + std::sin(angle) * radius;

            //Cette partie la est pour faire fonctionner les fonctions
            Enemy_Deer* cerf = new Enemy_Deer(startX, startY, false, textureCerf);
            cerf->orbitAngle = angle;//leur point de depart
            cerf->movementFunction = moveCircle;
            entities.push_back(cerf);
        }
    }
    // WAVE 4 SURVIVAL
    void SpawnWave4(float deltaTime) {
        survivalTimer += deltaTime;

        if (survivalTimer < survivalDuration) {
            meteorSpawnTimer += deltaTime;

            // Spawn plus rapide qu'en wave 2
            float spawnRate = 0.4f;

            if (meteorSpawnTimer >= spawnRate) {
                meteorSpawnTimer = 0.0f;

                // Pattern aléatoire parmi 3 types
                int pattern = rand() % 3;

                if (pattern == 0) {
                    // Rafale de 3 météorites en ligne
                    float baseX = static_cast<float>(rand() % 1600);
                    for (int i = 0; i < 3; i++) {
                        Enemy_Meteor* m = new Enemy_Meteor(baseX + i * 100.0f, -80.0f);
                        entities.push_back(m);
                    }
                }
                else if (pattern == 1) {
                    // Météorite diagonale (vitesse X ajoutée)
                    float spawnX = static_cast<float>(rand() % 1800);
                    Enemy_Meteor* m = new Enemy_Meteor(spawnX, -80.0f);
                    float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;
                    m->movement.velocity.x = direction * 150.0f; // Dérive latérale
                    entities.push_back(m);
                }
                else {
                    // Météorite rapide simple
                    float spawnX = static_cast<float>(rand() % 1800);
                    Enemy_Meteor* m = new Enemy_Meteor(spawnX, -80.0f);
                    m->movement.velocity.y *= 1.8f; // Plus rapide vers le bas
                    entities.push_back(m);
                }
            }
        } else {
            bool anyMeteorLeft = false;
            for (auto& ent : entities) {
                if (ent->entityType == EntityType::Enemy && !ent->bIsDestroyed) {
                    anyMeteorLeft = true;
                    break;
                }
            }
            if (!anyMeteorLeft) {
                PreparationNextWave();
            }
        }
    }

    //WAVE 5 BOSS PART 1
   void SpawnWave5() {

    }


//Fonction pour commencer une wave
    void StartWave(int wave) {
        GameApp &app = GameApp::GetInstance();
        waveInProgress = true;
        if (wave == 1) {
            //Les cerfs normaux
            currentWaveType = WaveType::Elimination;
            SpawnWave1();
        }
        else if (wave == 2) {
            //Les meteorites qui tombent
            currentWaveType = WaveType::Survival;
            survivalTimer = 0.0f;
            meteorSpawnTimer = 0.0f;
        }
        else if (wave == 3) {
            //cerfs qui bougent en cercle
            currentWaveType = WaveType::Elimination;
            SpawnWave3();
        }
        else if (wave == 4) {
            //meteorites qui tombent avec different pattern
            currentWaveType = WaveType::Survival;
            survivalTimer = 0.0f;
            meteorSpawnTimer = 0.0f;
        }
        else if (wave == 5) {
            currentWaveType = WaveType::Elimination;
            SpawnWave5();
        }
        else {
            waveInProgress = false; // stop la logique de wave
            app.StateActuel = State::NiveauGagnerScreen;
        }
    }
    //fonction de transition
    void PreparationNextWave() {
        waveInProgress = false;
        isTransitioning = true;
        transitionTimer = 0.0f;
        currentWave++; // On incrémente pour l'affichage
        waveFadeAlpha = 0.0f;
    }

    //Pour render tous les entitees dans Game && DeathScreen && Pause Screen
    void RenderEntities() {
// Dessiner toutes les entités
        for (auto &ent: entities) {
            if (Bullet* bulletCompetenceSpecial = dynamic_cast<Bullet*>(ent)) {
                if (bulletCompetenceSpecial->bIsRGB) {
                    bulletCompetenceSpecial->render.color = { r, g, b, 255 }; // r,g,b changent chaque frame
                }
            }

            // Rendu texture fraise
            if (BulletStrawberry* strawb = dynamic_cast<BulletStrawberry*>(ent)) {
                if (strawb->textureStrawb != nullptr) {
                    SDL_FRect dest = {
                        strawb->transform.position.x,
                        strawb->transform.position.y,
                        strawb->transform.size.x,
                        strawb->transform.size.y
                    };
                    SDL_RenderTextureRotated(
           renderer,
           strawb->textureStrawb,
           nullptr,
           &dest,
           strawb->rotationAngle,
           nullptr,
           SDL_FLIP_NONE
       );
                    continue;
                }
            }

            // Rendu texture meat
            if (Collectible_Meat* meatTexture = dynamic_cast<Collectible_Meat *>(ent)) {
                if (meatTexture->textureMeat != nullptr) {
                    SDL_FRect dest = {
                        meatTexture->transform.position.x,
                        meatTexture->transform.position.y,
                        meatTexture->transform.size.x,
                        meatTexture->transform.size.y
                    };
                    SDL_RenderTextureRotated(
           renderer,
           meatTexture->textureMeat,
           nullptr,
           &dest,
           meatTexture->rotationAngle,
           nullptr,
           SDL_FLIP_NONE
       );

                    continue;
                }
            }
            //Rendu texture Cerf
            if (Enemy_Deer *enemy_deer = dynamic_cast<Enemy_Deer*>(ent)) {
                if (enemy_deer->textureCerf != nullptr) {
                    SDL_FRect dest = {
                        enemy_deer->transform.position.x,
                        enemy_deer->transform.position.y,
                        enemy_deer->transform.size.x,
                        enemy_deer->transform.size.y
                    };

                    if (enemy_deer->bIsFlashing) {
                        // Calcule l'intensité du rouge selon le temps restant
                        float ratio = enemy_deer->hitFlashTimer / enemy_deer->hitFlashDuration;
                        Uint8 flashIntensity = static_cast<Uint8>(ratio * 200); // 0 à 200
                        SDL_SetTextureColorMod(enemy_deer->textureCerf, 255, 255 - flashIntensity, 255 - flashIntensity);
                    } else {
                        // Remet la couleur normale
                        SDL_SetTextureColorMod(enemy_deer->textureCerf, 255, 255, 255);
                    }

                    SDL_RenderTexture(renderer, enemy_deer->textureCerf, nullptr, &dest);
                    continue;
                }
            }
            //Pour le rendu des bullets
            if (Bullet* bullet = dynamic_cast<Bullet*>(ent)) {
                if (bullet->textureBullet != nullptr) {
                    SDL_FRect dest = {
                        bullet->transform.position.x,
                        bullet->transform.position.y,
                        bullet->transform.size.x,
                        bullet->transform.size.y
                    };
                    if (bullet->bIsRGB) {
                        SDL_SetTextureColorMod(bullet->textureBullet, r, g, b);//rgb sur la texture
                        bullet->render.color = { r, g, b, 255 };
                    }
                    else {
                        SDL_SetTextureColorMod(bullet->textureBullet, 255, 255, 255);
                    }
                    SDL_RenderTexture(renderer, bullet->textureBullet, nullptr, &dest);
                    continue; // ← skip le RenderUpdate coloré
                }
            }

            ent->RenderUpdate(renderer);
        }

    }



    // La fonction Game ne boucle
    void Game(float deltaTime) {
        SDL_Event GameEvents;
        GameApp &app = GameApp::GetInstance();
        //Ajout de la fonction UpdateBackgroundTint pour avoir le rgb
        //mis sur le text dynamicscoreText -> UpdateText avec TTF
        UpdateBackgroundTint(deltaTime);
        TTF_SetTextColor(dynamicscoreText, r, g, b, 255);
        TTF_SetTextColor(InventoryText, 255, 255, 255, 255);
        TTF_UpdateText(dynamicscoreText);
        TTF_UpdateText(dynamicPlayerHeal);
        TTF_UpdateText(dynamicShieldHPText); // Pour update le shield amount dans Game
        TTF_UpdateText(waveDynamicNumberText);//Pour update le texte du Wave
        TTF_UpdateText(InventoryText);
        //PREMIERE VAGUE
        //gestion des etats entre jeu et transition
        if (isTransitioning) {
            transitionTimer += deltaTime;
            showWaveUI = true;

            // Mise à jour du texte
            std::string NextWave = "Wave : " + std::to_string(currentWave);
            TTF_SetTextString(waveDynamicNumberText, NextWave.c_str(), 0);
            // 0 a 1.5 -> on met le fondu
            if (transitionTimer <= 1.5f) {
                app.waveFadeAlpha += (255.0f / 1.5f) * deltaTime;
                if (app.waveFadeAlpha > 255.0f) app.waveFadeAlpha = 255.0f;
            }
            // 1.5 a 3.0 -> On revient vers le CLAIR
            else if (transitionTimer <= 3.0f) {

                if (!waveInProgress) {
                    StartWave(currentWave);
                }

                app.waveFadeAlpha -= (255.0f / 1.5f) * deltaTime;
                if (app.waveFadeAlpha < 0.0f) app.waveFadeAlpha = 0.0f;
            }

            // fin du timer -> enleve le UI + le fading + Timer reset pour la prochaine transition
            if (transitionTimer >= 3.0f) {
                isTransitioning = false;
                showWaveUI = false;
                app.waveFadeAlpha = 0.0f;
                transitionTimer = 0.0f;
            }
        }
        else if (!waveInProgress)
        {
            PreparationNextWave();
        }
        if (waveInProgress) {
            if (currentWaveType == WaveType::Survival && waveInProgress)
            {
                // les waves survivals
                    if (currentWave == 2) {
                        SpawnWave2(deltaTime);
                    }
                    else if (currentWave == 4) {
                        SpawnWave4(deltaTime);
                    }

            }
            //logique d'elimination des entiters apres wave
            if (currentWaveType == WaveType::Elimination && waveInProgress) {
                bool anyEnemyAlive = false;
                for (auto& enemy : entities) {
                    if (enemy->entityType == EntityType::Enemy && !enemy->bIsDestroyed) {
                        anyEnemyAlive = true;
                        break;
                    }
                }

                if (!anyEnemyAlive) {
                    PreparationNextWave(); // On déclenche la transition
                }
            }
        }

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
                player->MovementUpdate(deltaTime);//Mouvement
                player->ShootUpdate(entities, (SDL_FPoint){0, -1}, deltaTime);
                player->UpdateCompetence(deltaTime); // <- pour l'update de la capacite special
                //Bordure d'ecran du joueur different des ennemies
                if (player->HasComponent(TRANSFORM)) {
                    if (player->transform.position.x <= 0.0f) {
                        player->transform.position.x = 0.0f;
                    }
                    //- sa size
                    else if (player->transform.position.x + player->transform.size.x > 1920.0f) {
                        player->transform.position.x = 1920.0f - player->transform.size.x;
                    }
                }
            }
            //FONCTION COLLISION ENTRE ENTITITY ET JOUEUR
            for (auto& entity : entities) {
                if (entity->bIsDestroyed) continue; // ignore ce qui est mort
                if (entity->entityType == EntityType::Player) continue;
                entity->Update(deltaTime);

                // Si c'est un ennemi
                if (entity->entityType == EntityType::Enemy) {
                    Enemy_Deer* deer = dynamic_cast<Enemy_Deer*>(entity);
                    if (deer != nullptr) {
                        deer->Update(deltaTime, entities, app.textureStrawberry); // Gère son tir et son mouvement
                    }
                }

                // Si fraise (EnemyBullet) ou Viande (Collectable) ou meteorite cerf (Enemy)
                if (entity->entityType == EntityType::Collectable || entity->entityType == EntityType::EnemyBullet || entity->entityType == EntityType::Enemy) {

                    // On crée les Rects pour la collision
                    SDL_FRect rectPlayer = { player->transform.position.x, player->transform.position.y, player->transform.size.x, player->transform.size.y };
                    SDL_FRect rectEnt = { entity->transform.position.x, entity->transform.position.y, entity->transform.size.x, entity->transform.size.y };

                    //si collision
                    if (SDL_HasRectIntersectionFloat(&rectPlayer, &rectEnt)) {

                        // -Si c'est une viante
                        if (entity->entityType == EntityType::Collectable) {
                            SDL_Log("Viande collectée -> +1");
                            currentMeat++;
                        }
                        // si Fraise
                        else if (entity->entityType == EntityType::EnemyBullet) {
                            int damage = 50;
                            // Si le shield a des HP restants, il absorbe en premier
                            if (player->currentShieldHP > 0) {
                                player->currentShieldHP -= damage;
                                if (player->currentShieldHP < 0) {
                                    // Le shield est detruit alors on passe au hp du joueur
                                    player->health.current_health += player->currentShieldHP; // currentShieldHP est negatif ici
                                    player->currentShieldHP = 0;
                                }
                            } else {
                                player->health.current_health -= damage;
                            }
                            currentHP = player->health.current_health;
                            if (player->health.current_health <= 0) {
                                player->health.current_health = 0;
                                PlayerDeath();
                            }
                        }
                        //Si toucher avec Enemy (Cerf ou Meteorite)
                        else if (entity->entityType == EntityType::Enemy) {
                            int damage = 100;
                            if (player->currentShieldHP > 0) {
                                player->currentShieldHP -= damage;
                                if (player->currentShieldHP < 0) {
                                    player->health.current_health += player->currentShieldHP;
                                    player->currentShieldHP = 0;
                                }
                            } else {
                                player->health.current_health -= damage;
                            }
                            currentHP = player->health.current_health;
                            if (player->health.current_health <= 0) {
                                player->health.current_health = 0;
                                PlayerDeath();
                            }
                        }

                        entity->bIsDestroyed = true; // Dans les deux cas, l'objet disparaît
                    }
                }
            }

            //Variables de Detection des cerfs et murs
            bool ToucheMurGauche = false;
            bool ToucheMurDroit = false;
            float MaxPushBack = 0.0f;

            // Mouvement et Détection Des cerfs
            for (auto &ent: entities) {
                if (ent->entityType == EntityType::Player) continue; //Le joueur a son propre mouvement alors il est exclue du Mouvements des autres. sinon il aurait 2 fois le mouvement appliquer
                ent->MovementUpdate(deltaTime);
                //On verifie que seulement les ennemies bougent pas joueur !@!@
                if (ent->entityType == EntityType::Enemy) {
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
            }

            // Application de la correction
            // touché à Gauche -> force vers la Droite
            if (ToucheMurGauche) {
                for (auto &ent: entities) {
                    //Verifie que c'est bien seulement les ennemis qui ont la correction
                    if (ent->entityType == EntityType::Enemy && ent->HasComponent(TRANSFORM)) {
                        //vitesse en positif std::abs (droite)
                        ent->movement.velocity.x = std::abs(ent->movement.velocity.x);
                        ent->transform.position.x += (MaxPushBack + 1.0f);
                    }
                }
            }
            // touché à Droite -> force vers la Gauche
            else if (ToucheMurDroit) {
                for (auto &ent: entities) {
                    //Verifie que c'est bien seulement les ennemis qui ont la correction
                    if (ent->entityType == EntityType::Enemy && ent->HasComponent(TRANSFORM)) {
                        // vitesse en Negatif -std::abs (gauche)
                        ent->movement.velocity.x = -std::abs(ent->movement.velocity.x);
                        ent->transform.position.x -= (MaxPushBack + 1.0f);
                    }
                }
            }

            //L'algorithme de collision entre bullet et cerf
            //vérifier chaque balle pour voir si elle touche un ennemi.
            for (auto &bullet: entities) {
                //recherche entiter qui est pas une balle
                if (bullet->entityType != EntityType::Bullet || bullet->bIsDestroyed) {
                    //non
                    continue;
                }
                for (auto &ennemi: entities) {
                    if (ennemi->entityType == EntityType::Enemy && !ennemi->bIsDestroyed) {
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
                            //Pv des ennemies baisses + Ajout d'un vfx rouge sur le cerf
                            ennemi->health.current_health -= 20;
                            // Déclenche le flash rouge
                            Enemy_Deer* deerHit = dynamic_cast<Enemy_Deer*>(ennemi);
                            if (deerHit != nullptr) {
                                deerHit->bIsFlashing = true;
                                deerHit->hitFlashTimer = deerHit->hitFlashDuration;
                            }

                            //si heal plus petit que 0 alors le cerf est detruit + score totaux
                            if (ennemi->health.current_health <= 0) {
                                ennemi->bIsDestroyed = true;
                                SDL_LogWarn(0, "Un cerf est mort");

                                //Ajout score
                                currentScore += scorePerDeerKilled;
                                SDL_LogWarn(0, "Le score est de %d", currentScore);
                                player->AddKillToCompetence();// Appel de la fonction AddKillToCompetence pour reduire le timer quand un cerf est mort

                                // On drop un collectible a collecter
                                //Creation Collision Bullet Entity
                                float spawnX = ennemi->transform.position.x + (ennemi->transform.size.x / 3);
                                float spawnY = ennemi->transform.position.y + (ennemi->transform.size.y / 3);
                                // On CREE LE COLLECTIBLE
                                entities.push_back(new Collectible_Meat(spawnX, spawnY, textureMeat));
                            }
                            break;
                        }
                    }
                }
            }

        //Pour Detruire Un objet après détruit
        for (auto enemyEntities = entities.begin(); enemyEntities != entities.end(); ) {

            if ((*enemyEntities)->bIsDestroyed) {
                delete *enemyEntities;              // libère memoire
                enemyEntities = entities.erase(enemyEntities); // pour retirer les cerfs de la liste pour eviter les troues
            } else {
                ++enemyEntities; // entité suivante
            }
        }

        //Pour afficher le score
        //Meme principe que pour le fps dynamic a afficher
        if (currentScore != lastScore) {
            std::string scoreStr = std::to_string(currentScore);
            TTF_SetTextString(dynamicscoreText, scoreStr.c_str(), 0);
            lastScore = currentScore;
        }
        //Pour afficher le nombre de Meat Dans Game
        if (currentMeat != lastMeat) {
            std::string meatStr = std::to_string(currentMeat);
            TTF_SetTextString(InventoryText, meatStr.c_str(), 0);
            lastMeat = currentMeat;
        }
        //Pour afficher les HP actuel dans le jeu
        if (player->health.current_health != lastHP) {
            currentHP = player->health.current_health;
            std::string healStr ="Heal : " + std::to_string(currentHP);
            TTF_SetTextString(dynamicPlayerHeal, healStr.c_str(), 0);
            lastHP = currentHP;
        }
        //Pour afficher les HP du shield dans le jeu
        if (player->currentShieldHP != lastShield) {
            std::string ShieldStr = "Shield Amount : " + std::to_string(player->currentShieldHP);
            TTF_SetTextString(dynamicShieldHPText, ShieldStr.c_str(), 0);
            lastShield = player->currentShieldHP;
        }
        // Rendu du jeu

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir pour le jeu
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, textureBackground, nullptr, nullptr);
        SDL_RenderTexture(renderer, ScoreUI, nullptr, &scoreSize);
        SDL_RenderTexture(renderer,HealUI,nullptr, &healSize);

      //  SDL_RenderFillRect(renderer, &MeatInventory);
        SDL_RenderTexture(renderer,textureMeat, nullptr, &MeatInventory);
        //Mis a jour du score rendu
        if (dynamicscoreText) {
            int longeurW, largeurH;
            TTF_GetTextSize(dynamicscoreText, &longeurW, &largeurH);
            TTF_DrawRendererText(dynamicscoreText, scoreSize.x + (scoreSize.w - longeurW)/2, scoreSize.y + (scoreSize.h - largeurH)- 20);
        }
        //Mise a jour du Meat rendu
        if (InventoryText) {
            int longeurL, largeurH;
            TTF_GetTextSize(InventoryText, &longeurL, &largeurH);
            float posX = MeatInventory.x + MeatInventory.w + 10.0f;
            float posY = MeatInventory.y + (MeatInventory.h - largeurH) / 4.0f;
            TTF_DrawRendererText(InventoryText, posX, posY);
        }
        //Mise a jour du Heal rendu
        if (dynamicPlayerHeal) {
            int longeurW, largeurH;
            TTF_GetTextSize(dynamicPlayerHeal, &longeurW, &largeurH);
            TTF_DrawRendererText(dynamicPlayerHeal, healSize.x + (healSize.w - longeurW)/2, healSize.y + (healSize.h - largeurH)- 20);
        }
        //Mise a jour du Shield Heal Rendu
        if (dynamicShieldHPText) {
            int longeurW, largeurH;
            TTF_GetTextSize(dynamicShieldHPText, &longeurW, &largeurH);
            TTF_DrawRendererText(dynamicShieldHPText, 75, 990);
        }




        //Optimisation seulement appel a RenderEtities pour Render les cerfs , fraises ...
        //Juste a appeler la fonction pour deathscreen, winscreen, Game
        RenderEntities();
        TTF_DrawRendererText(fpsText, 1800, 10); // Affiche FPS en jeu aussi
        // Barre de compétence spéciale
        SDL_FRect jaugeBg   = { 50.0f, 955.0f, 250.0f, 22.0f };
        float ratio = player->competenceTimer / player->competenceCooldown;
        SDL_FRect jaugeFill = { 50.0f, 955.0f, 250.0f * ratio, 22.0f };

        // Fond gris foncé
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderFillRect(renderer, &jaugeBg);

        // Remplissage
        if (player->bCompetenceActive) {
            TTF_DrawRendererText(competenceSpecialText2, 50, 925);
            TTF_SetTextColor(competenceSpecialText2,40,40,40,255);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255); // RGB animé
        } else if (player->bCompetenceReady) {
            //Rajouter Texte + Color ~
            TTF_DrawRendererText(competenceSpecialText, 50, 925);
            SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255); // Vert
        } else {
            TTF_DrawRendererText(competenceSpecialText2, 50, 925);
            TTF_SetTextColor(competenceSpecialText2,40,40,40,255);
            SDL_SetRenderDrawColor(renderer, 80, 80, 220, 255); // en recharge
        }
        SDL_RenderFillRect(renderer, &jaugeFill);

        // Barre shield

        SDL_FRect shieldBg   = { 50.0f, 1020.0f, 250.0f, 25.0f };
        float shieldRatio = (float)player->currentShieldHP / (float)player->maxShieldHP;
        SDL_FRect shieldFill = { 50.0f, 1020.0f, 250.0f * shieldRatio, 25.0f };

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderFillRect(renderer, &shieldBg);

        SDL_Color shieldColor = player->currentShield->GetColor();
        SDL_SetRenderDrawColor(renderer, shieldColor.r, shieldColor.g, shieldColor.b, 255);
        SDL_RenderFillRect(renderer, &shieldFill);



// Tout ce qui est en HAUT aura l'opaciter
        if (isTransitioning) {
            //Texte
            // la transparence
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            // couleur sur Noir
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)app.waveFadeAlpha);

            // le rectangle qui couvre
            SDL_FRect screenOverlay = { 0.0f, 0.0f, 1920.0f, 1080.0f };
            SDL_RenderFillRect(renderer, &screenOverlay);
        }
        //Mise a jour du numero de wave / Apres le fondu noir pour toujours etre visible
        if (showWaveUI) {
            if (waveDynamicNumberText) {
                int longeurW, largeurH;
                TTF_GetTextSize(waveDynamicNumberText, &longeurW, &largeurH);
                TTF_DrawRendererText(waveDynamicNumberText, 800, 400);
            }
        }



        SDL_RenderPresent(renderer);

    }

    //Fonction Lorsque le Joueur est mort
    void PlayerDeath() {
        GameApp &app = GameApp::GetInstance();

        app.player->health.current_health = 0;
        app.currentHP = 0;
        app.deathFadeAlpha = 0.0f; // La transparance commence a 0
        app.StateActuel = State::DeathScreen; // On change d'état

        SDL_Log("Transition vers l'écran de mort");
    }

    //Lorsque Joueur est mort
    void DeathScreen(float deltaTime) {
        GameApp &app = GameApp::GetInstance();

        // On remet le fond noir et on clear
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);
    //  BACKGROUND TEXTURE
        SDL_RenderTexture(renderer, textureBackground, nullptr, nullptr);
        //On dessine les entities et UI sans les faire bouger
        SDL_RenderTexture(renderer, ScoreUI, nullptr, &scoreSize);
        //Rajouter le score dynamique lors du Pause
        if (currentScore != lastScore) {
            std::string scoreStr = std::to_string(currentScore);
            TTF_SetTextString(dynamicscoreText, scoreStr.c_str(), 0);
            lastScore = currentScore;
        }
        if (dynamicscoreText) {
            int longeurW, largeurH;
            TTF_GetTextSize(dynamicscoreText, &longeurW, &largeurH);
            TTF_DrawRendererText(dynamicscoreText, scoreSize.x + (scoreSize.w - longeurW)/2, scoreSize.y + (scoreSize.h - largeurH)- 20);
        }





        //Légé changement à l'image
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);//Peux d'opaciter
        SDL_FRect screenRect = {0, 0, 1920, 1080};
        //Carre rouge pour viande

        SDL_RenderFillRect(renderer, &MeatInventory);
        SDL_RenderTexture(renderer,textureMeat, nullptr, &MeatInventory);
        //On remet en noir l'ecran ->
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
        SDL_RenderFillRect(renderer, &screenRect);
        //Mise a jour du Meat rendu
        if (InventoryText) {
            int longeurL, largeurH;
            TTF_GetTextSize(InventoryText, &longeurL, &largeurH);
            float posX = MeatInventory.x + MeatInventory.w + 10.0f;
            float posY = MeatInventory.y + (MeatInventory.h - largeurH) / 4.0f;
            TTF_DrawRendererText(InventoryText, posX, posY);
        }
        //Optimisation seulement appel a RenderEtities pour Render les cerfs , fraises ...
        //Juste a appeler la fonction pour deathscreen, winscreen, Game
        RenderEntities();

            // Barre de compétence spéciale
            SDL_FRect jaugeBg   = { 50.0f, 955.0f, 250.0f, 22.0f };
            float ratio = player->competenceTimer / player->competenceCooldown;
            SDL_FRect jaugeFill = { 50.0f, 955.0f, 250.0f * ratio, 22.0f };

            // Fond gris foncé
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            SDL_RenderFillRect(renderer, &jaugeBg);

            // Remplissage
            if (player->bCompetenceActive) {
                TTF_DrawRendererText(competenceSpecialText2, 50, 925);
                TTF_SetTextColor(competenceSpecialText2,40,40,40,255);
                SDL_SetRenderDrawColor(renderer, r, g, b, 255); // RGB animé
            } else if (player->bCompetenceReady) {
                //Rajouter Texte + Color ~
                TTF_DrawRendererText(competenceSpecialText, 50, 925);
                SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255); // Vert
            } else {
                TTF_DrawRendererText(competenceSpecialText2, 50, 925);
                TTF_SetTextColor(competenceSpecialText2,40,40,40,255);
                SDL_SetRenderDrawColor(renderer, 80, 80, 220, 255); // en recharge
            }
            SDL_RenderFillRect(renderer, &jaugeFill);

            // Barre shield

            SDL_FRect shieldBg   = { 50.0f, 1020.0f, 250.0f, 25.0f };
            float shieldRatio = (float)player->currentShieldHP / (float)player->maxShieldHP;
            SDL_FRect shieldFill = { 50.0f, 1020.0f, 250.0f * shieldRatio, 25.0f };

            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            SDL_RenderFillRect(renderer, &shieldBg);

            SDL_Color shieldColor = player->currentShield->GetColor();
            SDL_SetRenderDrawColor(renderer, shieldColor.r, shieldColor.g, shieldColor.b, 255);
            SDL_RenderFillRect(renderer, &shieldFill);

            //Le Texte du shield et PlayerHeal qui s'affiche
            if (dynamicPlayerHeal) {
                int longeurW, largeurH;
                TTF_GetTextSize(dynamicPlayerHeal, &longeurW, &largeurH);
                TTF_DrawRendererText(dynamicPlayerHeal, healSize.x + (healSize.w - longeurW)/2, healSize.y + (healSize.h - largeurH)- 20);
            }
            //Mise a jour du Shield Heal Rendu
            if (dynamicShieldHPText) {
                int longeurW, largeurH;
                TTF_GetTextSize(dynamicShieldHPText, &longeurW, &largeurH);
                TTF_DrawRendererText(dynamicShieldHPText, 75, 990);
            }


        TTF_DrawRendererText(fpsText, 1800, 10);


        //TOUT CE QUI EST DEVANT CERA TOUCHER PAR LE FONDU 
        // LE FONDU PROGRESSIF
        if (app.deathFadeAlpha < 300.0f) {
            app.deathFadeAlpha += (150.0f * deltaTime);
            if (app.deathFadeAlpha > 255.0f) app.deathFadeAlpha = 255.0f;
        }

        // Dessiner le fond noir a nouveau
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, (Uint8)app.deathFadeAlpha);
        SDL_RenderFillRect(app.renderer, &screenRect);

//si fondu est asser opaque
        if (app.deathFadeAlpha >= 200.0f) {
            //Le Texte et boutons
            UpdateBackgroundTint(deltaTime);
            RenderDeathTitle();
            if (selectedButtonDeath == 0) {
                RenderBoutons(BoutonDeathReturnMenu, DeathScreenReturnMenuText, r, g, b);
            }else {
                RenderBoutons(BoutonDeathReturnMenu, DeathScreenReturnMenuText, 40, 40, 40);
            }
        }
        SDL_RenderPresent(app.renderer);

    }
// VOID RESTART GAME A FAIRE POUR RECOMMENCER SI JOUEUR MORT ~~~~~

//VOID FONCTION QUAND LE JOUEUR GAGNE LE NIVEAU ET SES WAVES
void NiveauGagnerScreen(float deltaTime) {
GameApp &app = GameApp::GetInstance();
        //A configurer
        // On remet le fond noir et on clear
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        UpdateBackgroundTint(deltaTime);//<-- pour le rgb
        RenderWinTitle();
        //Boutons pour Gamepad
        if (selectedButtonWin == 0) {
            RenderBoutons(BoutonWinReturnMenu, WinScreenReturnMenuText, r,g,b);
        }else {
            RenderBoutons(BoutonWinReturnMenu, WinScreenReturnMenuText, 40,40,40);

        }
        TTF_DrawRendererText(fpsText, 1800, 10);
        SDL_RenderPresent(app.renderer);
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
        RenderScoreTitle();
        UpdateBackgroundTint(deltaTime);

        if (selectedButtonScore == 0) {
            RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, r, g, b);
        }else {
            RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, 250, 255, 255);
        }
        TTF_DrawRendererText(fpsText, 1800, 10); // Affiche FPS en jeu aussi

        SDL_RenderPresent(renderer);
    }

    //Avoir un shop pour acheter des skins -> amilioration d'arme
    void Shop(float deltaTime) {
        SDL_Event ShopEvents;




        //Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Fond noir
       //clean
        SDL_RenderClear(renderer);
        //Render
        RenderShopTitle();
        UpdateBackgroundTint(deltaTime);//Le rgb
        RenderGlobalWeaponProgresBar(805, 750);// Pour render les cubes pour les armes.
        RenderGlobalShieldProgresBar(1005, 750);// Pour render les cubes pour les shields
        //Bouton Upgrade Weapon
        if (selectedButtonShop == 0) {
            RenderBoutons(BoutonUpgrade, BoutonUpgradeText, r, g, b);
        }else {
            RenderBoutons(BoutonUpgrade, BoutonUpgradeText, 255, 255, 255);
        }//Bouton HP
        if (selectedButtonShop == 1) {
            RenderBoutons(BoutonShieldUpgrade, BoutonHPUpgradeText, r, g, b);
        }else {
            RenderBoutons(BoutonShieldUpgrade, BoutonHPUpgradeText, 255, 255, 255);
        }//Bouton Return GAme
        if (selectedButtonShop == 2) {
            RenderBoutons(BoutonResumeGameShop, resumeGameShopText, r, g, b);
        }else {
            RenderBoutons(BoutonResumeGameShop, resumeGameShopText, 255,255,255);
        }//Bouton Quit Menu
        if (selectedButtonShop == 3) {
            RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, r, g, b);
        }else {
            RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, 255, 255, 255);
        }

        //RENDER Fonts
        //pour actualiser le current meat
        std::string meatStr = " " + std::to_string(currentMeat);
        TTF_SetTextString(currentMeatShop, meatStr.c_str(), 0);
        TTF_DrawRendererText(currentMeatShop, 270, 100);

        TTF_DrawRendererText(fpsText, 1800, 10);
        TTF_DrawRendererText(ArmeFireText, 100,350);
        TTF_DrawRendererText(ArmeIceText, 100,450);
        TTF_DrawRendererText(ArmeTBDText, 100,550);
        TTF_DrawRendererText(ShieldSmallText, 100,750);
        TTF_DrawRendererText(ShieldMediumText, 100,850);
        TTF_DrawRendererText(ShieldLargeText, 100,950);
        //Fire
        if (globalWeaponLevel >= 1) {
            // possece deja l'arme
            TTF_SetTextString(statusFire, "EQUIPPED", 0);
            TTF_SetTextColor(statusFire, 0, 255, 0, 255); // reste vert
        }
        else if (currentMeat >= 10) {

            TTF_SetTextString(statusFire, "AVAILABLE", 0);
            TTF_SetTextColor(statusFire, 0, 255, 0, 255); // VERT (Achetable)
        }
        else {

            TTF_SetTextString(statusFire, "LEVEL 2", 0);
            TTF_SetTextColor(statusFire, 255, 0, 0, 255); // ROUGE
        }
        TTF_DrawRendererText(statusFire, 100, 350 + 40);

        // Ice
        if (globalWeaponLevel >= 2) {
            //possede deja l'arme
            TTF_SetTextString(statusIce, "EQUIPPED", 0);
            TTF_SetTextColor(statusIce, 0, 255, 0, 255); // Reste VERT car acquis
        }
        else if (currentMeat >= 50) {

            TTF_SetTextString(statusIce, "AVAILABLE", 0);
            TTF_SetTextColor(statusIce, 0, 255, 0, 255); // VERT (Achetable)
        }
        else {

            TTF_SetTextString(statusIce, "LEVEL 3", 0); //
            TTF_SetTextColor(statusIce, 255, 0, 0, 255); // ROUGE
        }
        TTF_DrawRendererText(statusIce, 100, 450 + 40);

        //WEAPON TBD
        if (globalWeaponLevel >= 3) {
            //Possede deja l'arme
            TTF_SetTextString(statusTbd, "EQUIPEED", 0);
            TTF_SetTextColor(statusTbd, 0, 255, 0, 255); // Reste VERT car acquis
        }
        else if (currentMeat >= 100) {

            TTF_SetTextString(statusTbd, "AVAILABLE", 0);
            TTF_SetTextColor(statusTbd, 0, 255, 0, 255); // VERT (Achetable)
        }
        else {

            TTF_SetTextString(statusTbd, "LEVEL 4", 0);
            TTF_SetTextColor(statusTbd, 255, 0, 0, 255); // ROUGE
        }
        TTF_DrawRendererText(statusTbd, 100, 550 + 40);

        //Small Shield
        if (globalShieldLevel >= 1) {
            // possece deja le shield
            TTF_SetTextString(statusSmallShield, "EQUIPPED", 0);
            TTF_SetTextColor(statusSmallShield, 0, 255, 0, 255); // reste vert
        }
        else if (currentMeat >= 10) {

            TTF_SetTextString(statusSmallShield, "AVAILABLE", 0);
            TTF_SetTextColor(statusSmallShield, 0, 255, 0, 255); // VERT (Achetable)
        }
        else {

            TTF_SetTextString(statusSmallShield, "LEVEL 2", 0);
            TTF_SetTextColor(statusSmallShield, 255, 0, 0, 255); // ROUGE
        }
        TTF_DrawRendererText(statusSmallShield, 100, 750 + 40);

        // Medium Shield
        if (globalShieldLevel >= 2) {
            //possede deja un shield
            TTF_SetTextString(statusMediumShield, "EQUIPPED", 0);
            TTF_SetTextColor(statusMediumShield, 0, 255, 0, 255); // Reste VERT car acquis
        }
        else if (currentMeat >= 50) {

            TTF_SetTextString(statusMediumShield, "AVAILABLE", 0);
            TTF_SetTextColor(statusMediumShield, 0, 255, 0, 255); // VERT (Achetable)
        }
        else {

            TTF_SetTextString(statusMediumShield, "LEVEL 3", 0); //
            TTF_SetTextColor(statusMediumShield, 255, 0, 0, 255); // ROUGE
        }
        TTF_DrawRendererText(statusMediumShield, 100, 850 + 40);

        //Large Shield
        if (globalShieldLevel >= 3) {
            //Possede deja un shield
            TTF_SetTextString(statusLargeShield, "EQUIPEED", 0);
            TTF_SetTextColor(statusLargeShield, 0, 255, 0, 255); // Reste VERT car acquis
        }
        else if (currentMeat >= 100) {

            TTF_SetTextString(statusLargeShield, "AVAILABLE", 0);
            TTF_SetTextColor(statusLargeShield, 0, 255, 0, 255); // VERT (Achetable)
        }
        else {

            TTF_SetTextString(statusLargeShield, "LEVEL 4", 0);
            TTF_SetTextColor(statusLargeShield, 255, 0, 0, 255); // ROUGE
        }
        TTF_DrawRendererText(statusLargeShield, 100, 950 + 40);


        //SDL_RenderFillRect(renderer, &MeatInventoryShop);
        SDL_RenderTexture(renderer,textureMeat, nullptr, &MeatInventoryShop);

        SDL_RenderPresent(renderer);
    }

    //Avoir un shop pour acheter des skins -> amilioration d'arme
    void Credits(float deltaTime) {
        SDL_Event ShopEvents;

        //Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Fond noir
        SDL_RenderClear(renderer);
        UpdateBackgroundTint(deltaTime);//Le rgb
        RenderCreditsTitle();
        //boutons
        if (selectedButtonCredits == 0) {
            RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, r, g, b);
        }else {
            RenderBoutons(BoutonQuitRetourMenu, TextQuitReturnMenu, 255, 255, 255);
        }


        //Fonts
        TTF_DrawRendererText(fpsText, 1800, 10);
        TTF_DrawRendererText(CreditsName1Text, 950,450);
        TTF_DrawRendererText(CreditsName2Text, 500,650);
        TTF_DrawRendererText(CreditsName3Text, 1400,650);
        TTF_DrawRendererText(CreditsRoleText, 850,350);
        TTF_DrawRendererText(CreditsRoleText2, 400, 550);
        TTF_DrawRendererText(CreditsRoleText3, 1300, 550);
        SDL_RenderPresent(renderer);
    }


    //Fonction pour mettre en pause le jeu
    void PauseSystem(float deltaTime) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
        //BACKGROUND
    SDL_RenderTexture(renderer, textureBackground, nullptr, nullptr);

    // On dessine les entities et UI sans les faire bouger
    SDL_RenderTexture(renderer, ScoreUI, nullptr, &scoreSize);

    // Rajouter le score dynamique lors du Pause
    if (currentScore != lastScore) {
        std::string scoreStr = std::to_string(currentScore);
        TTF_SetTextString(dynamicscoreText, scoreStr.c_str(), 0);
        lastScore = currentScore;
    }
    if (dynamicscoreText) {
        int longeurW, largeurH;
        TTF_GetTextSize(dynamicscoreText, &longeurW, &largeurH);
        TTF_DrawRendererText(dynamicscoreText, scoreSize.x + (scoreSize.w - longeurW)/2, scoreSize.y + (scoreSize.h - largeurH)- 20);
    }

    //Texture viande
    SDL_RenderFillRect(renderer, &MeatInventory);
    SDL_RenderTexture(renderer, textureMeat, nullptr, &MeatInventory);

    // Mise a jour du Meat rendu
    if (InventoryText) {
        int longeurL, largeurH;
        TTF_GetTextSize(InventoryText, &longeurL, &largeurH);
        float posX = MeatInventory.x + MeatInventory.w + 10.0f;
        float posY = MeatInventory.y + (MeatInventory.h - largeurH) / 4.0f;
        TTF_DrawRendererText(InventoryText, posX, posY);
    }

    UpdateBackgroundTint(deltaTime);

        //Optimisation seulement appel a RenderEtities pour Render les cerfs , fraises ...
        //Juste a appeler la fonction pour deathscreen, winscreen, Game
        RenderEntities();

        // Barre de compétence spéciale
        SDL_FRect jaugeBg   = { 50.0f, 955.0f, 250.0f, 22.0f };
        float ratio = player->competenceTimer / player->competenceCooldown;
        SDL_FRect jaugeFill = { 50.0f, 955.0f, 250.0f * ratio, 22.0f };

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderFillRect(renderer, &jaugeBg);

        if (player->bCompetenceActive) {
            TTF_DrawRendererText(competenceSpecialText2, 50, 925);
            TTF_SetTextColor(competenceSpecialText2, 40, 40, 40, 255);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        } else if (player->bCompetenceReady) {
            TTF_DrawRendererText(competenceSpecialText, 50, 925);
            SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);
        } else {
            TTF_DrawRendererText(competenceSpecialText2, 50, 925);
            TTF_SetTextColor(competenceSpecialText2, 40, 40, 40, 255);
            SDL_SetRenderDrawColor(renderer, 80, 80, 220, 255);
        }
        SDL_RenderFillRect(renderer, &jaugeFill);

        // Barre shield
        SDL_FRect shieldBg   = { 50.0f, 1020.0f, 250.0f, 25.0f };
        float shieldRatio = (float)player->currentShieldHP / (float)player->maxShieldHP;
        SDL_FRect shieldFill = { 50.0f, 1020.0f, 250.0f * shieldRatio, 25.0f };

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderFillRect(renderer, &shieldBg);

        SDL_Color shieldColor = player->currentShield->GetColor();
        SDL_SetRenderDrawColor(renderer, shieldColor.r, shieldColor.g, shieldColor.b, 255);
        SDL_RenderFillRect(renderer, &shieldFill);

        // Texte du shield et PlayerHeal
        if (dynamicPlayerHeal) {
            int longeurW, largeurH;
            TTF_GetTextSize(dynamicPlayerHeal, &longeurW, &largeurH);
            TTF_DrawRendererText(dynamicPlayerHeal, healSize.x + (healSize.w - longeurW)/2, healSize.y + (healSize.h - largeurH)- 20);
        }

        // Mise a jour du Shield Heal Rendu
        if (dynamicShieldHPText) {
            int longeurW, largeurH;
            TTF_GetTextSize(dynamicShieldHPText, &longeurW, &largeurH);
            TTF_DrawRendererText(dynamicShieldHPText, 75, 990);
        }

        TTF_DrawRendererText(fpsText, 1800, 10);
    // Rectangle de teinture sombre (TOUT CE QUI EST EN HAUT AURA LA TEINTE ->)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_FRect screenRect = {0, 0, 1920, 1080};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(renderer, &screenRect);

    // Boutons dessus la teinte
    if (selectedButtonPause == 0) {
        RenderBoutons(BoutonResume, TextResume, r, g, b);
    } else {
        RenderBoutons(BoutonResume, TextResume, 40, 40, 40);
    }

    if (selectedButtonPause == 1) {
        RenderBoutons(BoutonGoShop, TextPauseGoShop, r, g, b);
    } else {
        RenderBoutons(BoutonGoShop, TextPauseGoShop, 40, 40, 40);
    }

    if (selectedButtonPause == 2) {
        RenderBoutons(BoutonReturnMenu, TextReturnMenuPause, r, g, b);
    } else {
        RenderBoutons(BoutonReturnMenu, TextReturnMenuPause, 40, 40, 40);
    }


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

            case State::ChoixNiveau:
                ChoixNiveau(deltaTime);
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
            case State::Pause:
                PauseSystem(deltaTime);
                break;
                //pour acceder a l'intro avant de jouer
            case State::IntroNiveau1:
                IntroGame(deltaTime);
                break;
            case State::IntroNiveau2:
                IntroGame(deltaTime);
                break;
            case State::IntroNiveau3:
                IntroGame(deltaTime); // <- Intro game qui s'occupe de chaque intro differente
                break;
            case State::DeathScreen:
                DeathScreen(deltaTime);
                break;
            case State::NiveauGagnerScreen:
                NiveauGagnerScreen(deltaTime);
                break;

            case State::Quit:
                return SDL_APP_SUCCESS;
        }
        return SDL_APP_CONTINUE;
    }
};

//PLAN DE CONCEPTION COMMAND

//Classe pour mouvement
class MoveCommand : public Command {
    Player *player;
    bool bIsMoving;
    bool bMovingRight;
public:
    MoveCommand(Player *player, bool moving, bool right):player(player),bIsMoving(moving),bMovingRight(right){}
    void execute() override {
        if (bMovingRight) {
            player->bIsMovingRight = bIsMoving;
        }else {
            player->bIsMovingLeft = bIsMoving;
        }
    }
};
//Class Pour tirer
class ShootCommand : public Command {
    Player *player;
    bool bIsShooting;
public:
    ShootCommand(Player *player, bool shoot) : player(player), bIsShooting(shoot){}

    void execute() override {
        player->isCurrentlyShooting = bIsShooting;
    }
};



//Appeler seulement une seul fois
//-> Parfait pour SINGLETON
SDL_AppResult
//deux *pour modifier et ecrire dans le pointeur (post-it de l'objet qu;on peut mettre de l'information a l'interieur)				2 etoiles argv signifie avec un array
SDL_AppInit(void **appstate, int argc, char *argv[]) {
    //Avec le SINGLETON ->
    GameApp &app = GameApp::GetInstance();

    //LIER LES TOUCHES AUX COMMANDES DANS APPINIT
    //KEY DOWN
    app.keyBindings[SDL_SCANCODE_D] = new MoveCommand(app.player, true, true);//true moving + true moving right
    app.keyBindings[SDL_SCANCODE_A] = new MoveCommand(app.player, true, false);//true moving + false -> not going right (moving left)
    app.keyBindings[SDL_SCANCODE_SPACE] = new ShootCommand(app.player,true);
    //KEY UP
    app.keyReleaseBindings[SDL_SCANCODE_D] = new MoveCommand(app.player, false, true);
    app.keyReleaseBindings[SDL_SCANCODE_A] = new MoveCommand(app.player, false, false);
    app.keyReleaseBindings[SDL_SCANCODE_SPACE] = new ShootCommand(app.player, false);


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
        //POUR POWER UP
        if (app.StateActuel == State::Game) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_WEST) {

                SDL_Log("Button Down");
                //POWER UP
                app.player->ActivateCompetence(app.textureBulletNormal);
            }
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_START) {
                app.StateActuel = State::Pause;
            }
        }
        //Dans Le IntroNIveau1
        if (app.StateActuel == State::IntroNiveau1) {
            app.indexMessageIntroNiveau1++;

            if (app.indexMessageIntroNiveau1 < app.NB_MESSAGES_IntroNiveau1) {

                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau1[app.indexMessageIntroNiveau1], 0);
            } else {
                app.StateActuel = State::Game;
                app.indexMessageIntroNiveau1 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau1[0], 0);
            }
        }
        //Dans Le Introniveau2
        if (app.StateActuel == State::IntroNiveau2) {
            app.indexMessageIntroNiveau2++;

            if (app.indexMessageIntroNiveau2 < app.NB_MESSAGES_IntroNiveau2) {

                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau2[app.indexMessageIntroNiveau2], 0);
            } else {
                app.StateActuel = State::Game;
                app.indexMessageIntroNiveau2 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau2[0], 0);
            }
        }
        //Dans Le Introniveau 3
        if (app.StateActuel == State::IntroNiveau3) {
            app.indexMessageIntroNiveau3++;

            if (app.indexMessageIntroNiveau3 < app.NB_MESSAGES_IntroNiveau3) {

                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau3[app.indexMessageIntroNiveau3], 0);
            } else {
                app.StateActuel = State::Game;
                app.indexMessageIntroNiveau3 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau3[0], 0);
            }
        }



        //GERER SELECTION MENU AVEC GAMEPAD
        if (app.StateActuel == State::Menu) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN) {
                //Par en bas on augmente le num du menu (passe de 0 a 1 -> de Play a Score)
                app.selectedButtonMenu++;
                //Dessend 0 vers 4~
                if (app.selectedButtonMenu > 4) {
                    app.selectedButtonMenu = 0;//loop entre 4 et 0
                }
            }
            //Monte ~
                if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) {
                //Monte
                app.selectedButtonMenu--;
                if (app.selectedButtonMenu < 0) {
                    app.selectedButtonMenu = 4;
                }
            }
            //validation Touche A
            if (event->gbutton.button ==  SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("blablabla");

            //switch case MENU
            switch (app.selectedButtonMenu) {
                case 0:
                    app.StateActuel = State::ChoixNiveau;
                    break;
                case 1:
                    app.StateActuel = State::ScoreBoard;
                    break;
                case 2:
                    app.StateActuel = State::Shop;
                    break;
                case 3:
                    app.StateActuel = State::Quit;
                    app.selectedButtonShop = 0;//Remet a zero
                    break;
                case 4:
                    app.StateActuel = State::Credits;
                    app.selectedButtonShop = 0;
                    break;
                }

            }


        }
        //GERER LES BOUTONS CHOIXNIVEAU
        else if (app.StateActuel == State::ChoixNiveau) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN) {
                //Par en bas on augmente le num du menu (passe de 0 a 1 -> de Play a Score)
                app.selectedButtonChoixNiveau++;
                //Dessend 0 vers 2~ 3 choix
                if (app.selectedButtonChoixNiveau > 2) {
                    app.selectedButtonChoixNiveau = 0;//loop entre 3 et 0
                }
            }
            //Monte ~
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) {
                //Monte
                app.selectedButtonChoixNiveau--;
                if (app.selectedButtonChoixNiveau < 0) {
                    app.selectedButtonChoixNiveau = 2;
                }
            }
            //validation Touche A
            if (event->gbutton.button ==  SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("blablabla");

                //switch case ChoixNiveau
                switch (app.selectedButtonChoixNiveau) {
                    case 0:
                        app.indexMessageIntroNiveau1 = 0;
                        TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau1[0], 0);
                        app.StateActuel = State::IntroNiveau1;
                        break;
                    case 1:
                        app.indexMessageIntroNiveau2 = 0;
                        TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau2[0], 0);
                        app.StateActuel = State::IntroNiveau2; // <-A FAIRE LES DIFFERENTS INTRO
                        break;
                    case 2:
                        app.indexMessageIntroNiveau3 = 0;
                        TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau3[0], 0);
                        app.StateActuel = State::IntroNiveau3; // <- A FAIRE LES DIFFERENTS INTRO
                        break;
                }

            }


        }


        else if (app.StateActuel == State::Pause) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN){
                app.selectedButtonPause++;
                if (app.selectedButtonPause > 2) {
                    app.selectedButtonPause = 0;//Retourne au premier
                }
            }
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) {
                app.selectedButtonPause--;
                if (app.selectedButtonPause < 0) {
                    app.selectedButtonPause = 2;//retourne au dernier
                }
            }

            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("Button A Down");
                //SwitchCase
                switch (app.selectedButtonPause) {
                    case 0:
                        //Resume
                        //Rien Encore Pour L'upgrade
                        SDL_Log("Resume");
                        app.StateActuel = State::Game;
                        break;
                    case 1:
                        //Return Shop
                        SDL_Log("Go dans le shop");
                        app.StateActuel = State::Shop;
                        app.selectedButtonPause = 0; // Pour que le selectedButton commence toujours par 0 -> Resume
                        break;
                    case 2:
                        //Return Menu
                        SDL_Log("Retour Menu");
                        app.StateActuel = State::Menu;
                        app.selectedButtonPause = 0; // Pour que le selectedButton commence toujours par 0 -> Resume
                        break;
                }
            }
        }
        //GERER SELECTION WinSCREEN GAMEPAD
        else if (app.StateActuel == State::NiveauGagnerScreen) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN) {
                //app.selectedButtonWin++;
                if (app.selectedButtonWin > 0) {
                    app.selectedButtonWin = 0;//Retourne au premier
                }
            }
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) {
                //app.selectedButtonWin --;
                if (app.selectedButtonWin < 0) {
                    app.selectedButtonWin = 0;
                }
            }
            //Verification
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("Button A Down");
                //SwitchCase
                switch (app.selectedButtonWin) {
                    case 0:

                        SDL_Log("Retour Menu");
                        app.StateActuel = State::Menu;
                        break;
                }
            }
        }


        //GERER SELECTION DEATHSCREEN GAMEPAD
        else if (app.StateActuel == State::DeathScreen) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN) {
                //app.selectedButtonDeath++;
                if (app.selectedButtonDeath > 0) {
                    app.selectedButtonDeath = 0;//Retourne au premier
                }
            }
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) {
                //app.selectedButtonDeath --;
                if (app.selectedButtonDeath < 0) {
                    app.selectedButtonDeath = 0;
                }
            }
            //Verification
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("Button A Down");
                //SwitchCase
                switch (app.selectedButtonDeath) {
                    case 0:
                        //Rien Encore Pour L'upgrade
                        SDL_Log("Retour Menu");
                        app.StateActuel = State::Menu;
                        break;
                }
            }
        }


        //GERER SELECTION SCORE AVEC GAMEPAD
        else if (app.StateActuel == State::ScoreBoard) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN){
                //app.selectedButtonScore++;
                if (app.selectedButtonScore > 0) {
                    app.selectedButtonScore = 0;//Retourne au premier
                }
            }
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) {
                //app.selectedButtonScore--;
                if (app.selectedButtonScore < 0) {
                    app.selectedButtonScore = 0;//retourne au dernier
                }
            }
            //Verification
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("Button A Down");
                //SwitchCase
                switch (app.selectedButtonCredits) {
                    case 0:
                        //Rien Encore Pour L'upgrade
                        SDL_Log("Retour Menu");
                        app.StateActuel = State::Menu;
                        break;
                }
            }
        }

        //GERER SELECTION SHOP AVEC GAMEPAD
        else if (app.StateActuel == State::Shop) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT){
            app.selectedButtonShop++;
                if (app.selectedButtonShop > 3) {
                    app.selectedButtonShop = 0;//Retourne au premier
                }
            }
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT) {
                app.selectedButtonShop--;
                if (app.selectedButtonShop < 0) {
                    app.selectedButtonShop = 3;//retourne au dernier
                }
            }
            //Verification
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("Button A Down");
                //SwitchCase
                switch (app.selectedButtonShop) {
                    case 0:
                        //Rien Encore Pour L'upgrade
                        SDL_Log("Weapon Upgrade");
                        // player->ArmeUpgrade();
                        if (app.currentWeaponLevel == 0) {
                            if (app.player->ArmeUpgrade(ArmeNiveau::Fire, app.currentMeat)) {
                                app.currentWeaponLevel = 1;// on achete la prochaine arme
                                app.globalWeaponLevel = 1;
                            }
                        }
                        else if (app.currentWeaponLevel == 1){
                            if (app.player->ArmeUpgrade(ArmeNiveau::Ice, app.currentMeat)) {
                                app.currentWeaponLevel = 2;
                                app.globalWeaponLevel = 2;
                            }
                        }
                        else if (app.currentWeaponLevel == 2) {
                            if (app.player->ArmeUpgrade(ArmeNiveau::Tbd, app.currentMeat)) {
                                app.currentWeaponLevel = 3;
                                app.globalWeaponLevel = 3;
                            }
                        }
                        break;
                    case 1:
                        //Rien Encore Pour Upgrade HP
                        SDL_Log("HP Upgrade");
                        break;
                    case 2:
                        //Retour menu
                        app.StateActuel = State::Game;
                        app.selectedButtonShop = 0;
                        break;
                    case 3:
                        app.StateActuel = State::Menu;
                        app.selectedButtonShop = 0;
                        break;
                }
            }
        }
        
        //CREDITS
        else if (app.StateActuel == State::Credits) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN){
                //app.selectedButtonCredits++;
                if (app.selectedButtonCredits > 0) {
                    app.selectedButtonCredits = 0;//Retourne au premier
                }
            }
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) {
                //app.selectedButtonCredits--;
                if (app.selectedButtonCredits < 0) {
                    app.selectedButtonCredits = 0;//retourne au dernier
                }
            }
            //Verification
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
                SDL_Log("Button A Down");
                //SwitchCase
                switch (app.selectedButtonCredits) {
                    case 0:
                        //Rien Encore Pour L'upgrade
                        SDL_Log("Retour Menu");
                        app.StateActuel = State::Menu;
                        break;
                }
            }
        }

        //PLEIN ECRAN MANETTE
        if (event->gbutton.button == SDL_GAMEPAD_BUTTON_BACK) {
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


    }
    // BOUTON MANETTE UP
    if (event->type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
        if (app.StateActuel == State::Game) {
            if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {

                SDL_Log("Button Up");
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
                    app.player->bIsMovingRight = true;
                    app.player->bIsMovingLeft = false;
                }
                // Vers la gauche
                else if (xValue < -app.DEADZONE) {
                    app.player->bIsMovingRight = false;
                    app.player->bIsMovingLeft = true;
                }
                // deadZone
                else {
                    app.player->bIsMovingRight = false;
                    app.player->bIsMovingLeft = false;
                }
            }
        }
        else if (event->gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
            Sint16 triggerValue = event->gaxis.value;

            if (triggerValue > 3000) { // > deadZone Du Trigger
                if (app.StateActuel == State::Game) {
                    app.player->isCurrentlyShooting = true;
                }
            }
            else {
                // Si on relâche la gâchette
                if (app.StateActuel == State::Game) {
                    app.player->isCurrentlyShooting = false;
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
                app.StateActuel = State::ChoixNiveau;
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
        //DAns le ChoixNiveau
        else if (app.StateActuel == State::ChoixNiveau) {
            //va vers intro de niveau 1
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonChoixNiveau1)) {
                app.indexMessageIntroNiveau1 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau1[0], 0);
                app.StateActuel = State::IntroNiveau1;
            }
            //va vers intro de niveau 2
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonChoixNiveau2)) {
                app.indexMessageIntroNiveau2 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau2[0], 0);
                app.StateActuel = State::IntroNiveau2;
            }
            //va vers intro de niveau 3
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonChoixNiveau3)) {
                app.indexMessageIntroNiveau3 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau3[0], 0);
                app.StateActuel = State::IntroNiveau3;
            }
        }
        //Dans Le IntroNiveau1
        else if (app.StateActuel == State::IntroNiveau1) {
            app.indexMessageIntroNiveau1++;

            if (app.indexMessageIntroNiveau1 < app.NB_MESSAGES_IntroNiveau1) {
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau1[app.indexMessageIntroNiveau1], 0);
            } else {
                app.StateActuel = State::Game;
            }

        }
        //Dans le IntroNiveau2
        else if (app.StateActuel == State::IntroNiveau2) {
        app.indexMessageIntroNiveau2++;

            if (app.indexMessageIntroNiveau2 < app.NB_MESSAGES_IntroNiveau2) {
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau2[app.indexMessageIntroNiveau2], 0);
            }
            else {
                app.StateActuel = State::Game;
            }
        }
        //Dans le Intro Niveau3
        else if (app.StateActuel == State::IntroNiveau3) {
        app.indexMessageIntroNiveau3++;
            if (app.indexMessageIntroNiveau3 < app.NB_MESSAGES_IntroNiveau3) {
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau3[app.indexMessageIntroNiveau3], 0);
            }
        }
        //Dans Le Pause
        else if (app.StateActuel == State::Pause) {
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonResume)) {
                app.StateActuel = State::Game;
            }
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonGoShop)) {
                app.StateActuel = State::Shop;
            }

            if (SDL_PointInRectFloat(&MousePT, &app.BoutonReturnMenu)) {
                app.StateActuel = State::Menu;
            }
        }
        //DANS LE WINSCREEN
        else if (app.StateActuel == State::NiveauGagnerScreen) {
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonWinReturnMenu)) {
                app.StateActuel = State::Menu;
            }
        }

        //DANS LE DEATHSCREEN
        else if (app.StateActuel == State::DeathScreen) {
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonDeathReturnMenu)) {
                app.StateActuel = State::Menu;
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
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonResumeGameShop)) {
                app.StateActuel = State::Game;
            }
            // Bouton Upgrade Arme
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonUpgrade)) {
                SDL_Log("Achat Upgrade Arme");
                // player->ArmeUpgrade();
                if (app.currentWeaponLevel == 0) {
                    if (app.player->ArmeUpgrade(ArmeNiveau::Fire, app.currentMeat)) {
                        app.currentWeaponLevel = 1;// on achete la prochaine arme
                        app.globalWeaponLevel = 1;
                        app.player->currentWeapon->texture = app.textureBulletFire; //Change les textures en Feu
                    }
                }
                else if (app.currentWeaponLevel == 1){
                    if (app.player->ArmeUpgrade(ArmeNiveau::Ice, app.currentMeat)) {
                        app.currentWeaponLevel = 2;
                        app.globalWeaponLevel = 2;
                        app.player->currentWeapon->texture = app.textureBulletIce; //Change les textures en glace
                    }
                }
                else if (app.currentWeaponLevel == 2) {
                    if (app.player->ArmeUpgrade(ArmeNiveau::Tbd, app.currentMeat)) {
                        app.currentWeaponLevel = 3;
                        app.globalWeaponLevel = 3;
                        app.player->currentWeapon->texture = app.textureBulletNormal;//Normal pour l'instant
                        //TBD
                    }
                }
            }

            // Bouton Upgrade HP
            if (SDL_PointInRectFloat(&MousePT, &app.BoutonShieldUpgrade)) {
                SDL_Log("Achat Upgrade HP");
                // player->ShieldUpgrade();
                if (app.currentShieldLevel == 0) {
                    if (app.player->ShieldUpgrade(ShieldAmount::SmallShield, app.currentMeat)) {
                        app.currentShieldLevel = 1; // On achete le prochain shield
                        app.globalShieldLevel = 1;
                    }
                }
                else if (app.currentShieldLevel == 1) {
                    if (app.player->ShieldUpgrade(ShieldAmount::MediumShield, app.currentMeat)) {
                        app.currentShieldLevel = 2;
                        app.globalShieldLevel = 2;
                    }
                }
                else if (app.currentShieldLevel == 2) {
                    if (app.player->ShieldUpgrade(ShieldAmount::LargeShield, app.currentMeat)) {
                        app.currentShieldLevel = 3;
                        app.globalShieldLevel = 3;
                    }
                }
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
        if (event->key.scancode == SDL_SCANCODE_P) {
            if (app.StateActuel==State::Game) {
                app.StateActuel = State::Pause;
            }
            else if (app.StateActuel == State::Pause) {
                app.StateActuel = State::Game;
            }

        }
        //Dans Le IntroGame
         if (app.StateActuel == State::IntroNiveau1) {
            app.indexMessageIntroNiveau1++;

            if (app.indexMessageIntroNiveau1 < app.NB_MESSAGES_IntroNiveau1) {

                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau1[app.indexMessageIntroNiveau1], 0);
            } else {
                app.StateActuel = State::Game;
                app.indexMessageIntroNiveau1 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau1[0], 0);
            }
        }
        //Dans Le Introniveau2
        if (app.StateActuel == State::IntroNiveau2) {
            app.indexMessageIntroNiveau2++;

            if (app.indexMessageIntroNiveau2 < app.NB_MESSAGES_IntroNiveau2) {

                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau2[app.indexMessageIntroNiveau2], 0);
            } else {
                app.StateActuel = State::Game;
                app.indexMessageIntroNiveau2 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau2[0], 0);
            }
        }
        //Dans Le Introniveau 3
        if (app.StateActuel == State::IntroNiveau3) {
            app.indexMessageIntroNiveau3++;

            if (app.indexMessageIntroNiveau3 < app.NB_MESSAGES_IntroNiveau3) {

                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau3[app.indexMessageIntroNiveau3], 0);
            } else {
                app.StateActuel = State::Game;
                app.indexMessageIntroNiveau3 = 0;
                TTF_SetTextString(app.texteIntroCerfEtHUmain, app.phrasesIntroNiveau3[0], 0);
            }
        }

        if (app.StateActuel == State::Game) {

            if (app.keyBindings.count(event->key.scancode)) {
                app.keyBindings[event->key.scancode]->execute();
            }
            if (event->key.scancode == SDL_SCANCODE_E) {
                app.player->ActivateCompetence(app.textureBulletNormal);
            }
            /*ancienne methode
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
            */
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

            if (app.keyReleaseBindings.count(event->key.scancode)) {
                app.keyReleaseBindings[event->key.scancode]->execute();
            }
            /*
            //Relache D
            if (event->key.scancode == SDL_SCANCODE_D) {
                player->bIsMovingRight = false;
            }
            //Relache A
            if (event->key.scancode == SDL_SCANCODE_A) {
                player->bIsMovingLeft = false;
            }
            if (event->key.scancode == SDL_SCANCODE_SPACE) {
            player->isCurrentlyShooting = false;
            }
            */
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
//test

/*

//Main de facon traditionnel
int main(int argc, char *argv[])
{

	// instance unique du Singleton
	GameApp::GetInstance().Run();

l	return 0;
}
*/


