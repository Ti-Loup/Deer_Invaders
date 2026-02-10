//defines
#define SDL_MAIN_USE_CALLBACKS//Pour le main


//includes
#include <SDL3/SDL.h>
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




static constexpr Sint32 TILE_SIZE = 32;
static constexpr Sint32 ANIM_ROW_BEGIN = 0;
static constexpr Sint32 ANIM_ROW_END = 0;
static constexpr Sint32 ANIM_COL_BEGIN = 0;
static constexpr Sint32 ANIM_COL_END = 6;
static constexpr Sint32 PRESENT_SIZE = 8;

static Uint32 TimerCallback(void *userdata, SDL_TimerID timerID, Uint32 interval)
{
	bool *updateFlag = static_cast<bool *>(userdata);
	*updateFlag = true;
	return interval;
}

class GameApp {

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
		TTF_TextEngine *textEngine = nullptr;
		TTF_Text *fpsText = nullptr;
		TTF_Text *MenuTitle = nullptr;//Pour rajouter un Titre
	//Texte special
		TTF_Font *MenuSpecialFont = nullptr;
		TTF_Text *MenuSpecialDraw = nullptr;
	//creation des boutons pour le menu
		SDL_FRect BoutonPlay = { 50, 450, 250, 100 };
		bool bClickedOnPlay = false;
		bool bClickedOnQuit = false;
		bool bClickedOnScore = false;
		SDL_FRect BoutonScore = { 120, 600, 250, 100 };
		SDL_FRect BoutonQuit = { 180, 750, 250, 100 };

		TTF_Text *TextStart = nullptr;
		TTF_Text *TextQuit = nullptr;
		TTF_Text *TextScore = nullptr;//Pour les boutons Start Quit et TBh
		TTF_Text *TextQuitScore = nullptr;
		TTF_Font *BoutonFont = nullptr;

		// -> GAME <- Text et Fonts

		// -> Score <- Text et Fonts
		TTF_Font *ReturnBoutonFont = nullptr;
		SDL_FRect BoutonQuitScore = { 1250, 900, 250, 100 };


		std::vector<float> frameTimes;
		const size_t MAX_SAMPLES = 100;
		bool shouldUpdateText = false;
		SDL_TimerID fpsTimerID;
		float currentFPS = 0.0f;

		// La liste de toutes les entités du jeu
		std::vector<Entity*> entities;

	public:
		GameApp()//Constructeur
		{
			if (SDL_Init(SDL_INIT_VIDEO) == false)
				{
					SDL_LogCritical(1, "SDL failed to initialize! %s", SDL_GetError());
					abort();
				}
			window = SDL_CreateWindow("Deer Invaders", 1600, 1080, 0);
			if (window == nullptr)
				{
					SDL_LogCritical(1, "SDL failed to create window! %s", SDL_GetError());
					abort();
				}
			renderer = SDL_CreateRenderer(window, nullptr);
			if (renderer == nullptr)
				{
					SDL_LogCritical(1, "SDL failed to create renderer! %s", SDL_GetError());
					abort();
				}
			spritesheet = IMG_LoadTexture(renderer, "assets/spritesheet.png");
			if (spritesheet == nullptr)
				{
					SDL_LogWarn(0, "SDL_image failed to load texture '%s'! %s", "assets/spritesheet.png",
								SDL_GetError());
				}
			SDL_SetTextureScaleMode(spritesheet, SDL_SCALEMODE_NEAREST);
			if (TTF_Init() == false)
				{
					SDL_LogCritical(1, "SDL_ttf failed to initialize! %s", SDL_GetError());
					abort();
				}
			textEngine = TTF_CreateRendererTextEngine(renderer);
			if (textEngine == nullptr)
				{
					SDL_LogCritical(1, "SDL_ttf failed to create text engine!! %s", SDL_GetError());
					abort();
				}
			font = TTF_OpenFont("assets/font.ttf", 24);
			if (font == nullptr)
				{
					SDL_LogWarn(0, "SDL_ttf failed to load font '%s'! %s", "assets/font.ttf", SDL_GetError());
				}
			fpsText = TTF_CreateText(textEngine, font, "FPS: 0", 20);
			if (fpsText == nullptr)
				{
					SDL_LogWarn(0, "SDL_ttf failed to create text '%s'! %s", "FPS: 0", SDL_GetError());
				}
			if (TTF_SetTextColor(fpsText, 255, 255, 255, 255) == false)
				{
					SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
				}
			//Le font du Titre
			MenuFont = TTF_OpenFont("assets/New Space.ttf", 72);
			if (MenuFont == nullptr)
			{
				SDL_LogWarn(0, "SDL_ttf failed to load font '%s'! %s", "assets/New Space.ttf", SDL_GetError());
			}
			//Rajoue du menu
			MenuTitle = TTF_CreateText(textEngine, MenuFont, "         Deer \nInvaders", 25);

			if (MenuTitle == nullptr) {
				SDL_LogWarn(0,"Le menu n'a pas chargé : TTF",SDL_GetError());
			}
			if (TTF_SetTextColor(MenuTitle, 255, 255, 255, 255) == false)
			{
				SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
			}
			//special font
			MenuSpecialFont = TTF_OpenFont("assets/Space.ttf", 120)  ;
			if (MenuSpecialFont == nullptr) {
				SDL_LogWarn(0, "SDL_ttf failed to put the font", SDL_GetError());
			}
			MenuSpecialDraw = TTF_CreateText(textEngine, MenuSpecialFont, "abcdefghigklmnop", 20);

			BoutonFont = TTF_OpenFont("assets/New Space.ttf", 48);
			ReturnBoutonFont = TTF_OpenFont("assets/New Space.ttf", 24);
			{
				SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
			}
			TextStart = TTF_CreateText(textEngine, BoutonFont, "Start",25);
			if (MenuSpecialDraw == nullptr) {
				SDL_LogWarn(0,"Les boutons du menu n'a pas chargé : TTF",SDL_GetError());
			}
			TextQuit = TTF_CreateText(textEngine, BoutonFont, "Quit",25);
			if (TextQuit == nullptr) {
				SDL_LogWarn(0,"Les boutons du menu n'a pas chargé : TTF",SDL_GetError());
			}
			TextScore = TTF_CreateText(textEngine, BoutonFont, "Score",25);
			if (TextScore == nullptr) {
				SDL_LogWarn(0,"Les boutons du menu n'a pas chargé : TTF",SDL_GetError());
			}
			if (TTF_SetTextColor(TextStart, 0, 0, 0, 255) == false)
			{
				SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
			}
			if (TTF_SetTextColor(TextQuit, 0, 0, 0, 255) == false)
			{
				SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
			}
			if (TTF_SetTextColor(TextScore, 0, 0, 0, 255) == false)
			{
				SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
			}
			TextQuitScore = TTF_CreateText(textEngine, BoutonFont, "Return \nMenu",25);
			if (TextQuitScore == nullptr) {
				SDL_LogWarn(0,"SDL_TTF failed to set the return Menu text)! %s", SDL_GetError());
			}
			if (TTF_SetTextColor(TextQuitScore, 0, 0, 0, 255) == false)
			{
				SDL_LogWarn(0, "SDL_ttf failed to set color TextQuitScore %s", SDL_GetError());
			}
			//Ligne 1 de cerf
			entities.push_back(new Enemy_Deer(100.f, 100.0f));
			entities.push_back(new Enemy_Deer(250.f, 100.0f));
			entities.push_back(new Enemy_Deer(400.f, 100.0f));
			entities.push_back(new Enemy_Deer(550.0f, 100.0f));

			entities.push_back(new Enemy_Deer(950.0f, 100.0f));
			entities.push_back(new Enemy_Deer(1100.0f, 100.0f));
			entities.push_back(new Enemy_Deer(1250.0f, 100.0f));
			entities.push_back(new Enemy_Deer(1400.0f, 100.0f));

			//Ligne 2 de cerfs
			entities.push_back(new Enemy_Deer(100.f, 250.0f));
			entities.push_back(new Enemy_Deer(250.f, 250.0f));
			entities.push_back(new Enemy_Deer(400.f, 250.0f));
			entities.push_back(new Enemy_Deer(550.0f, 250.0f));

			entities.push_back(new Enemy_Deer(950.0f, 250.0f));
			entities.push_back(new Enemy_Deer(1100.0f, 250.0f));
			entities.push_back(new Enemy_Deer(1250.0f, 250.0f));
			entities.push_back(new Enemy_Deer(1400.0f, 250.0f));
			//Ligne 3 de cerfs
			entities.push_back(new Enemy_Deer(100.f, 400.0f));
			entities.push_back(new Enemy_Deer(250.f, 400.0f));
			entities.push_back(new Enemy_Deer(400.f, 400.0f));
			entities.push_back(new Enemy_Deer(550.0f, 400.0f));

			entities.push_back(new Enemy_Deer(950.0f, 400.0f));
			entities.push_back(new Enemy_Deer(1100.0f, 400.0f));
			entities.push_back(new Enemy_Deer(1250.0f, 400.0f));
			entities.push_back(new Enemy_Deer(1400.0f, 400.0f));
			//Ligne 4 de cerfs
			entities.push_back(new Enemy_Deer(100.f, 600.0f));
			entities.push_back(new Enemy_Deer(250.f, 600.0f));
			entities.push_back(new Enemy_Deer(400.f, 600.0f));
			entities.push_back(new Enemy_Deer(550.0f, 600.0f));

			entities.push_back(new Enemy_Deer(950.0f, 600.0f));
			entities.push_back(new Enemy_Deer(1100.0f, 600.0f));
			entities.push_back(new Enemy_Deer(1250.0f, 600.0f));
			entities.push_back(new Enemy_Deer(1400.0f, 600.0f));

			fpsTimerID = SDL_AddTimer(250, TimerCallback, &shouldUpdateText);
		}
		//Libere memoire
		~GameApp()
		{
			SDL_RemoveTimer(fpsTimerID);
			TTF_DestroyText(fpsText);
			TTF_DestroyText(MenuTitle);
			TTF_DestroyRendererTextEngine(textEngine);
			TTF_CloseFont(font);
			TTF_CloseFont(BoutonFont);
			TTF_DestroyText(TextStart);
			TTF_DestroyText(TextQuit);
			TTF_DestroyText(TextScore);
			TTF_CloseFont(MenuSpecialFont);
			SDL_DestroyTexture(spritesheet);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
		}

		void CalculateFPS(const float deltaTime)
		{
			frameTimes.push_back(deltaTime);
			if (frameTimes.size() > MAX_SAMPLES)
				{
					frameTimes.erase(frameTimes.begin());
				}
			const float sum = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f);
			const float avgDelta = sum / static_cast<float>(frameTimes.size());
			currentFPS = (avgDelta > 0) ? 1.0f / avgDelta : 0;

			if (shouldUpdateText)
				{
					std::string fpsStr = "FPS: " + std::to_string(static_cast<int>(currentFPS));
					TTF_SetTextString(fpsText, fpsStr.c_str(), 0);
					shouldUpdateText = false; // Reset the flag
				}
		}

		void AdvanceAnimation(const float deltaTime)
		{
			animTimer += deltaTime;
			if (animTimer >= frameDuration)
				{
					animTimer = 0.0f;
					currentCol++;

					if (currentCol > ANIM_COL_END)
						{
							currentCol = ANIM_COL_BEGIN;
							currentRow++;
						}

					if (currentRow > ANIM_ROW_END)
						{
							currentRow = ANIM_ROW_BEGIN;
						}
				}
		}

		void RenderAnimation() const
		{
			if (spritesheet != nullptr)
				{
					const SDL_FRect src = {
							static_cast<float>(currentCol * TILE_SIZE),
							static_cast<float>(currentRow * TILE_SIZE),
							static_cast<float>(TILE_SIZE),
							static_cast<float>(TILE_SIZE),
					};

					constexpr SDL_FRect dst = {
							(2800.0f / 2.0f) - ((TILE_SIZE * PRESENT_SIZE) / 2.0f),
							(1700.0f / 2.0f) - ((TILE_SIZE * PRESENT_SIZE) / 2.0f),
							static_cast<float>((TILE_SIZE * PRESENT_SIZE)),
							static_cast<float>((TILE_SIZE * PRESENT_SIZE)),
					};

					SDL_RenderTexture(renderer, spritesheet, &src, &dst);
				}
		}
		//Petite fonction pour mettre un titre
		void RenderTitle() {

			TTF_DrawRendererText(MenuTitle, 0, 200);
			TTF_DrawRendererText(MenuSpecialDraw, 000, 950);
		}
		//Boutons
		void RenderBoutons(const SDL_FRect& rect, TTF_Text* buttonText, Uint8 buttonr, Uint8 buttong, Uint8 buttonb) {
			SDL_SetRenderDrawColor(renderer, buttonr, buttong, buttong, 255);
			SDL_RenderFillRect(renderer, &rect);
			//Dessiner Texte au centre du boutton
			if (buttonText != nullptr) {
				int textW, textH;//Longeur/Largeur
				TTF_GetTextSize(buttonText, &textW, &textH);


				float textX = rect.x + (rect.w - textW) / 2.0f;
				float textY = rect.y + (rect.h - textH) / 2.0f;

				TTF_DrawRendererText(buttonText, textX, textY);
			}

		}



	//Menu du jeu qui run
	void Menu(float deltaTime) {
		SDL_Event MenuEvents;
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

		// Rendu du menu
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);

		RenderTitle();
		RenderAnimation();

		// Mise à jour animation
		AdvanceAnimation(deltaTime);

		// Dessin des boutons
		RenderBoutons(BoutonPlay, TextStart, 250, 255, 255);
		RenderBoutons(BoutonQuit, TextQuit, 250, 255, 255);
		RenderBoutons(BoutonScore, TextScore, 250, 255, 255);

		TTF_DrawRendererText(fpsText, 1500, 10);
		SDL_RenderPresent(renderer);
	}

	// 3. MODIFICATION : La fonction Game ne boucle plus non plus
	void Game(float deltaTime) {
		SDL_Event GameEvents;
		while (SDL_PollEvent(&GameEvents)) {
			if (GameEvents.type == SDL_EVENT_QUIT) {
				StateActuel = State::Quit;
			}
			//Les touches
		}

			// Creation du update
			for (auto& ent : entities) {
				ent->MovementUpdate(deltaTime);
			}


		// Rendu du jeu
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir pour le jeu
		SDL_RenderClear(renderer);

			// Dessiner toutes les entités
			for (auto& ent : entities) {
				ent->RenderUpdate(renderer);
			}
		TTF_DrawRendererText(fpsText, 1500, 10); // Affiche FPS en jeu aussi

		SDL_RenderPresent(renderer);
	}
	//fonction pour la section score
	void Score(float deltaTime) {
			SDL_Event ScoreEvents;
			while (SDL_PollEvent(&ScoreEvents)) {
				if (ScoreEvents.type == SDL_EVENT_MOUSE_BUTTON_DOWN && ScoreEvents.button.button == SDL_BUTTON_LEFT) {
					SDL_FPoint MousePT = { ScoreEvents.button.x, ScoreEvents.button.y };

					// Si on clique sur QUIT
					if (SDL_PointInRectFloat(&MousePT, &BoutonQuitScore)) {
						StateActuel = State::Menu;
					}
				}

			}
			// Rendu du Score
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
			SDL_RenderClear(renderer);

			RenderBoutons(BoutonQuitScore, TextQuitScore, 250, 255, 255);
			TTF_DrawRendererText(fpsText, 1500, 10); // Affiche FPS en jeu aussi

			SDL_RenderPresent(renderer);
		}

	//L'execution ce fait ici avec un switch et les differents States

	void Run() {
		uint64_t lastTime = SDL_GetTicks();

		while (StateActuel != State::Quit) {
			// Calcul du temps global
			const uint64_t currentTime = SDL_GetTicks();
			const float deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
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

				case State::Quit:
					break;
			}
		}
	}
};

/*
SDL_AppResult //deux *pour modifier et ecrire dans le pointeur				2 etoiles argv signifie avec un array
SDL_AppInit (void **appstate, int argc, char *argv[]) {


	GameApp *app = new GameApp();


	*appstate = app;
	return SDL_APP_CONTINUE;
}
SDL_AppResult
SDL_AppEvent(void *appstate, SDL_Event *event) {


	return SDL_APP_CONTINUE;
}
SDL_AppResult
SDL_AppIterate (void *appstate){

GameApp *app = (GameApp *)appstate;
if (app) {
	app->Run();
}
	return SDL_APP_CONTINUE;
}

void
SDL_AppQuit (void *appstate, SDL_AppResult result)
{
}
*/


//Main


int main(int argc, char *argv[])
{

	GameApp app;

	app.Run();

	return 0;
}
