#define SDL_MAIN_HANDLED  

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "game.h"
#include "constants.h"
#include "GameUI.h"


int main() {
	Game* game = new Game();

	game->Init();

	while (game->Running()) {
		Uint32 frameStart = SDL_GetTicks();
		int frameTime;

		game->HandleEvents();
		game->Update();
		game->Render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < FRAME_DELAY) {
			SDL_Delay(FRAME_DELAY - frameTime);
		}
	}

	game->Clean();
	return 0;
}

