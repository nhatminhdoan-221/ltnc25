#pragma once
#ifndef GAME_UI_H
#define GAME_UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Board; 
void renderGameOverText(SDL_Renderer* renderer, TTF_Font* font, Board* board);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y);

#endif
