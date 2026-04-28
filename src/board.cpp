#include "board.h"
#include "queue.h"

#include <SDL.h>

Board::Board() {
    queue = new Queue();
    currentPiece = new Tetromino();
    queue->generateNewBag();
    queue->previewQueueUpdate();
    currentPiece->generateNewPiece(queue);
    clearBoard();
    score = 0;
    level = 1;
    linesCleared = 0;
}

int** Board::getPlayingField() {
    return reinterpret_cast<int**>(pBoard);
}

void Board::clearBoard() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            pBoard[i][j] = { 0 };
        }
    }
}

/*id Board::printBoard() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            std::cout << pBoard[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}*/

void Board::getRenderColor(SDL_Renderer* renderer, int piece_id) {
    switch (piece_id) {
    case 0:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);      //blank cells are black
        break;
    case 1:
        SDL_SetRenderDrawColor(renderer, 1, 237, 250, 255);  //I piece : CYAN
        break;
    case 2:
        SDL_SetRenderDrawColor(renderer, 72, 93, 197, 255);  //L piece: BLUE
        break;
    case 3:
        SDL_SetRenderDrawColor(renderer, 255, 200, 46, 255); //J piece: ORANGE
        break;
    case 4:
        SDL_SetRenderDrawColor(renderer, 253, 63, 89, 255);  //S piece: RED
        break;
    case 5:
        SDL_SetRenderDrawColor(renderer, 83, 218, 63, 255);  //Z piece: GREEN
        break;
    case 6:
        SDL_SetRenderDrawColor(renderer, 221, 10, 178, 255); //T piece: PURPLE
        break;
    case 7:
        SDL_SetRenderDrawColor(renderer, 254, 251, 52, 255); //:oyes:
        break;
    default:
        std::cerr << "Undefined behavior detected";
        exit(1);
    }
}

void Board::Render(SDL_Renderer* renderer) {
    //render the last 2 lines without the board grid(otherwise the piece would just look weird)
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (pBoard[i][j] != 0) {
                SDL_Rect cell = { BOARD_OFFSET_X + j * CELL_SIZE, BOARD_OFFSET_Y + i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

                getRenderColor(renderer, pBoard[i][j]);

                SDL_RenderFillRect(renderer, &cell);

                // Draw grid
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                SDL_RenderDrawRect(renderer, &cell);
            }
        }
    }

    //render the first 20 lines
    for (int i = 2; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            SDL_Rect cell = { BOARD_OFFSET_X + j * CELL_SIZE, BOARD_OFFSET_Y + i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

            getRenderColor(renderer, pBoard[i][j]);

            SDL_RenderFillRect(renderer, &cell);

            // Draw grid
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}


void Board::editCell(int i, int j, int val) {
    pBoard[i][j] = val;
}

bool Board::canMove(int newX, int newY, int newRotation) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Piece_Shape[currentPiece->pieceID][newRotation][i][j] != 0) {  // If this part of the piece is occupied
                int boardX = newX + j;
                int boardY = newY + i;

                // Check if the piece is out of bounds
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    //std::cout << "Couldnt move/rotate because out of bound";
                    return false;  // Block would go outside the board
                }

                // Check if the cell is already occupied
                if (pBoard[boardY][boardX] != 0) {
                    //std::cout << "Couldnt move/rotate because of collision";
                    return false;  // Block collides with an existing piece
                }
            }
        }
    }
    return true;
}


void Board::moveRight() {
    int newX = currentPiece->x + 1;
    deleteOldBlock();
    if (!Board::canMove(newX, currentPiece->y, currentPiece->rotation)) return;
    currentPiece->moveRight();
}

void Board::moveLeft() {
    int newX = currentPiece->x - 1;
    deleteOldBlock();
    if (!Board::canMove(newX, currentPiece->y, currentPiece->rotation)) return;
    currentPiece->moveLeft();
}

void Board::Rotate() {
    int currentRotation = currentPiece->rotation;
    int newRotation = (currentRotation + 1) % 4; 
    // Tinh trang thai xoay theo chieu kim dong ho

    deleteOldBlock(); // Xoa tetrimino cu

    // Th? xoay mà không di chuy?n
    if (canMove(currentPiece->x, currentPiece->y, newRotation)) {
        currentPiece->rotate(1); // Xoay neu khong co va cham
        putBlockInPlace(); // Ve lai tetrimino moi
        return;
    }

    // Neu xoay khong va cham, nudge len tren 1 don vi
    if (canMove(currentPiece->x, currentPiece->y - 1, newRotation)) {
        currentPiece->y--;
        currentPiece->rotate(1);
        putBlockInPlace();
        return;
    }

    // Van va cham sau khi nudge, nudge len tren 2 don vi
    if (canMove(currentPiece->x, currentPiece->y - 2, newRotation)) {
        currentPiece->y -= 2;
        currentPiece->rotate(1);
        putBlockInPlace();
        return;
    }

    // Neu khong xoay duoc, hoan tac viec xoay 
    putBlockInPlace();
}

void Board::ReversedRotate() {
    int currentRotation = currentPiece->rotation;
    int newRotation = (currentRotation - 1 + 4) % 4;

    deleteOldBlock(); // Xoa tetromino o vi tri cu

    int originalX = currentPiece->x;
    int originalY = currentPiece->y;

    currentPiece->rotate(-1); // Tam thoi xoay de kiem tra

    // Kiem tra va cham o vi tri hin tai sau khi xoay
    if (canMove(currentPiece->x, currentPiece->y, currentPiece->rotation)) {
        putBlockInPlace();
        return;
    }

    // Thu nudge len tren
    if (canMove(currentPiece->x, currentPiece->y - 1, currentPiece->rotation)) {
        currentPiece->y--;
        putBlockInPlace();
        return;
    }

    // Thunudge sang trai
    if (canMove(currentPiece->x - 1, currentPiece->y, currentPiece->rotation)) {
        currentPiece->x--;
        putBlockInPlace();
        return;
    }

    // Thunudge sang phai
    if (canMove(currentPiece->x + 1, currentPiece->y, currentPiece->rotation)) {
        currentPiece->x++;
        putBlockInPlace();
        return;
    }

    // Thu nudge len tren 2 don vi
    if (canMove(currentPiece->x, currentPiece->y - 2, currentPiece->rotation)) {
        currentPiece->y -= 2;
        putBlockInPlace();
        return;
    }

    // Neu khong tim duoc vi tri hop le, hoan tac viec xoay chuyen
    currentPiece->rotation = currentRotation;
    currentPiece->x = originalX;
    currentPiece->y = originalY;
    putBlockInPlace();
}




void Board::HardDrop() {
    int x = currentPiece->x;
    int y = currentPiece->y;
    int rotation = currentPiece->rotation;
    deleteOldBlock();
    while (canMove(x, y + 1, rotation)) {
        y++;
    }
    currentPiece->setPosition(x, y);
    putBlockInPlace();
    currentPiece->isLocked = true;
}

void Board::SoftDrop() {
    int x = currentPiece->x;
    int y = currentPiece->y;
    int rotation = currentPiece->rotation;
    deleteOldBlock();
    if (canMove(x, y + 1, rotation)) y++;
    currentPiece->setPosition(x, y);
}

void Board::deleteOldBlock() {
    if (currentPiece == nullptr || currentPiece->isLocked) return;

    int id = currentPiece->pieceID;
    int r = currentPiece->rotation;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int boardX = currentPiece->x + j;
            int boardY = currentPiece->y + i;

            if (Piece_Shape[id][r][i][j] != 0 &&
                boardX >= 0 && boardX < BOARD_WIDTH &&
                boardY >= 0 && boardY < BOARD_HEIGHT) {

                pBoard[boardY][boardX] = 0;
            }
        }
    }
}


void Board::putBlockInPlace() {
    if (currentPiece == nullptr || currentPiece->isLocked) return;

    int id = currentPiece->pieceID;
    int r = currentPiece->rotation;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int boardX = currentPiece->x + j;
            int boardY = currentPiece->y + i;

            if (Piece_Shape[id][r][i][j] != 0 &&
                boardX >= 0 && boardX < BOARD_WIDTH &&
                boardY >= 0 && boardY < BOARD_HEIGHT) {

                pBoard[boardY][boardX] = id + 1;  // or Piece_Shape[id][r][i][j];
            }
        }
    }
}


int Board::checkForLineClear() {
    int lineCleared = 0;

    for (int i = 2; i < BOARD_HEIGHT; i++) {
        bool isFullLine = true;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (pBoard[i][j] == 0) {
                isFullLine = false;
                break;
            }
        }

        if (isFullLine) {
            std::cout << "Line clear detected at line#" << i << "\n";
            lineCleared++;

            // shift all rows above down
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    pBoard[k][j] = pBoard[k - 1][j];
                }
            }

            // clear the top row
            for (int j = 0; j < BOARD_WIDTH; j++) {
                pBoard[0][j] = 0;
            }
        }
    }

    if (lineCleared > 0) {
        linesCleared += lineCleared;
        int baseScore = 0;
        switch (lineCleared) {
        case 1: baseScore = 10; break;
        case 2: baseScore = 30; break;
        case 3: baseScore = 60; break;
        case 4: baseScore = 100; break;
        }
        score += baseScore * (level + 9.0)/10; // Tinh diem theo tung level
    }

    return lineCleared;
}


void Board::GameOver() {
    std::cout << "Game over";
    isGameOver = true;
}

void Board::boardUpdate() {
    if (isGameOver || currentPiece == nullptr) return;

    Uint32 currentTime = SDL_GetTicks();

    // If this current block is locked -> create new block
    if (currentPiece->isLocked) {
        int clears = checkForLineClear(); // clear the line if it has

        // Check for level up
        if (linesCleared >= level * 20 && level < 20) {
            level++;
            std::cout << "Level Up! Current Level: " << level << "\n";
            // Them logic de thay doi toc do roi
        }

        currentPiece->generateNewPiece(queue);

        if (!canMove(currentPiece->x, currentPiece->y, 0)) {
            GameOver();
        }

        return; // stop and not create any new line
    }

    // automatic drop base on the time
    if (currentTime - lastDropTime >= baseDropInterval / speedMultiplier) {
        deleteOldBlock();

        int x = currentPiece->x;
        int y = currentPiece->y;
        int rotation = currentPiece->rotation;

        if (canMove(x, y + 1, rotation)) {
            currentPiece->setPosition(x, y + 1);
        }
        else {
            putBlockInPlace();
            currentPiece->isLocked = true;
        }

        lastDropTime = currentTime;

        // speed up till the highest speed
        if (speedMultiplier < 5.0f) {
            speedMultiplier += 0.000125f;
        }
    }

    // Only redraw if it is not locked
    if (!currentPiece->isLocked) {
        putBlockInPlace();
    }
}

void Board::Restart() {
    clearBoard(); // Xoa ban choi
    queue->generateNewBag(); // Tao mot bao (bag) va cac tetromino moi
    queue->previewQueueUpdate(); // Cap nhat hang doi xem truoc
    currentPiece->generateNewPiece(queue); // Tao mot trtromino moi
    isGameOver = false; // Dat lai trang thai game over
    lastDropTime = SDL_GetTicks(); // Dat lai thoi gian roi gan nhat
    speedMultiplier = 1.0f; // Dat lai toc do roi
    score = 0;          // Reset diem so
    level = 1;          // Reset level 
    linesCleared = 0;   // Reset so hang da pha
    linesCounted = 0;   // Reset bien dem hang
    
}

 
