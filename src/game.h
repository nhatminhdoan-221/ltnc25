
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "board.h"



class Game {
public:

	enum GameState { //for the menu
		MENU,
		PLAYING,
		GAME_OVER
	};

	Game();
	~Game();

	void Init();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
	bool Running() { return isRunning; };
	void Restart();

private:
	const char* title;
	int width;
	int height;
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Board* board;
	TTF_Font* font = nullptr; //font trong game
	TTF_Font* gameOverFont = nullptr; //font khi Game Over
	Uint32 startTime; //dong ho dem gio
	int elapsedTime; //THOI GIAN DA TROI QUA
	GameState currentGameState; //trang thai
	bool isSoundOn;
	SDL_Texture* menuBackgroundTexture; // luu texture hinh nen menu

	struct MenuButton {
		std::string text;
		SDL_Rect rect;
	};

	std::vector<MenuButton> menuButtons;

	int currentMenuItemIndex; //kiem tra muc nao trong menu duoc chon

	Mix_Music* backgroundMusic;
};

#endif //GAME_H
