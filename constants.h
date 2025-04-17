
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL.h>
#include <string>

const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS;

static const char* WINDOW_TITLE = "TETRIS VERSION ?";
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 1000;
const int BOARD_HEIGHT = 22;
const int BOARD_WIDTH = 10;
const int CELL_SIZE = 30;

//Board position(cell at 0,0)
const int BOARD_OFFSET_X = SCREEN_WIDTH / 2 - 200;
const int BOARD_OFFSET_Y = SCREEN_HEIGHT / 2 - 300;


/*
SDL_Color COLOR_RED			= { 253, 63, 89 };
SDL_Color COLOR_ORAGE		= { 255, 200, 46 };
SDL_Color COLOR_YELLOW		= { 254, 251, 52 };
SDL_Color COLOR_GREEN		= { 83, 218, 63 };
SDL_Color COLOR_CYAN		= { 1, 237, 250 };
SDL_Color COLOR_PURPLE		= { 221, 10, 178 };
SDL_Color COLOR_BLUE		= { 72, 93, 197 };
*/

const int PIECE_PREVIEW = 3;

const int INITIAL_PIECE_POSITION_X = 4;
const int INITIAL_PIECE_POSITION_Y = 1;
const int INITIAL_ROTATION_STATE = 0;

const int BASE_GRAVITY = 40;
const int GRAVITY_INCREMENT = 2;

const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 60;
const int BUTTON_SPACING = 20;



#endif#pragma once
