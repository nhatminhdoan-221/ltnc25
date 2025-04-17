#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <SDL.h>
#include <map>

#include "constants.h"
#include "queue.h"
#include "tetromino.h"

class Board {
public:

	Board();
	int** getPlayingField();
	void clearBoard();
	//id printBoard();
	void getRenderColor(SDL_Renderer* renderer, int piece_id);
	void Render(SDL_Renderer* renderer);
	void editCell(int i, int j, int val);
	bool canMove(int x, int y, int r);
	void moveRight();
	void moveLeft();
	void Rotate();
	void ReversedRotate();
	void HardDrop();
	void SoftDrop();
	void deleteOldBlock();
	void putBlockInPlace();
	int  checkForLineClear();
	void boardUpdate();
	void GameOver();
	bool isGameOverState() const { return isGameOver;}
	void Restart();
	int score;
	int level;
	int linesCleared;
	int linesCounted;

private:
	int pBoard[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
	int fTimer;
	bool isGameOver = false;
	Queue* queue = NULL;
	Tetromino* currentPiece = NULL;

	Uint32 lastDropTime = 0;              // Thoi diem roi gan nhat
	float speedMultiplier = 1.0f;         // Tang toc dan
	const Uint32 baseDropInterval = 1000; // Mac dinh: 1s moi o

};


#endif
