
#ifndef TETROMINO_H
#define TETROMINO_H

#include "pieces.h"
#include "constants.h"
#include "queue.h"

struct Tetromino {
    int x, y;
    int rotation;
    int pieceID;
    bool isLocked = false;

    void generateNewPiece(Queue*);  // Tao ra mieng moi dua tren queue
    int  getPieceID();
    void rotate(int dr);  // Lat 90 do
    void setPosition(int x, int y);  // Vi tri cua tetromino do 
    void setRotation(int& rotation);  // Dat goc quay cua tetromino
    void moveLeft();
    void moveRight();
    void moveDown();
};

#endif#pragma once
