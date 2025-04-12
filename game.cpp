#include "SDL.h"
#include <iomanip>
#include <sstream> 
#include <iostream>
#include <SDL_ttf.h>
#include "game.h"
#include "board.h"
#include "GameUI.h"

Game::Game() {
    title = WINDOW_TITLE;
    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;
    isRunning = false;
    window = nullptr;
    renderer = nullptr;
    board = new Board(); 
    font = nullptr;
    gameOverFont = nullptr;
}

Game::~Game() {
    delete board;
    TTF_CloseFont(font);
    TTF_CloseFont(gameOverFont);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    std::cerr << "Game exited!" << std::endl;
}

void Game::Init() {

    startTime = SDL_GetTicks();
    elapsedTime = 0;
    title = WINDOW_TITLE;
    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;

    // Khoi tao SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cerr << "SDL Initialized!" << std::endl;

    // Khoi tao SDL ttf de hien chu
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        isRunning = false;
        return;
    }

    // Tao cua so
    window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cerr << "Window created!" << std::endl;

    // Tao renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cerr << "Renderer created!" << std::endl;

    // Load font chu cho score va level 
    font = TTF_OpenFont("PressStart2P-Regular.ttf", 24);  // size 24 
    if (!font) {
        std::cerr << "Failed to load font (score/level): " << TTF_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cerr << "Font (score/level) loaded successfully!" << std::endl;

    // Load font chu cho Game Over 
    gameOverFont = TTF_OpenFont("PressStart2P-Regular.ttf", 24); 
    if (!gameOverFont) {
        std::cerr << "Failed to load font (Game Over): " << TTF_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cerr << "Font (Game Over) loaded successfully!" << std::endl;


    isRunning = true;
}

void Game::Restart() {
    // Goi ham Restart cua class Board de restart bai choi
    board->Restart();
    // Dat lai is Running neu can
    isRunning = true;
    std::cout << "Game::Restart() called, isRunning = " << isRunning << std::endl;

}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        if (board->isGameOverState()) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;

                // Toa do cua Restart lay tu vi tri cua renderGameOverText
                int buttonWidth = 200;
                int buttonHeight = 60;
                int centerX = SCREEN_WIDTH / 2;
                int startY = SCREEN_HEIGHT / 4;
                int offsetY = 50; // Dam bao gia tri giong voi trong renderGameOverText
                startY += offsetY * 2; // Sau "GAME OVER"
                startY += offsetY;     // Sau "Score:"
                startY += offsetY;     // Sau "Level:"
                startY += offsetY * 2; // Sau "Lines:"
                int restartButtonY = startY;
                int buttonSpacing = 20;
                int restartButtonX = centerX - buttonWidth / 2 - buttonSpacing - buttonWidth / 2;

                // Kiem tra nut chuot da click duoc chuan vao restart chua
                if (mouseX >= restartButtonX && mouseX < restartButtonX + buttonWidth &&
                    mouseY >= restartButtonY && mouseY < restartButtonY + buttonHeight) {
                    std::cout << "Restart buttion is clicked" << std::endl;
                    board->Restart(); // Goi ham restart cua Board
                }
            
       
                // Logic cho nut Home
                int homeButtonX = centerX + buttonSpacing / 2; //Khop vi tri Home voi vi tri ve
                int homeButtonY = startY; // ngang vi tri Restart

                if (mouseX >= homeButtonX && mouseX < homeButtonX + buttonWidth &&
                    mouseY >= homeButtonY && mouseY < homeButtonY + buttonHeight) {
                    std::cout << "Home button is clicked" << std::endl;
                    isRunning = false; // isRunning = false -> thoat game
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_r) {
                    std::cout << "R is clicked" << std::endl;
                    Restart(); // Goi ham Restart cua Game
                }
            }
        }
        else {
            // Cac phim bam trong game
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    board->moveLeft();
                    break;
                case SDLK_RIGHT:
                    board->moveRight();
                    break;
                case SDLK_DOWN:
                    board->SoftDrop();
                    break;
                case SDLK_UP:
                    board->Rotate();
                    break;
                case SDLK_z:
                    board->ReversedRotate();
                    break;
                case SDLK_SPACE:
                    board->HardDrop();
                    break;
                }
            }
        }
    }
}

void Game::Update() {
    Uint32 currentTime = SDL_GetTicks();
    elapsedTime = (currentTime - startTime) / 1000; // tinh thoi gian troi theo giay
    board->boardUpdate();
}



void Game::Render() {
    if (board->isGameOverState()) {
        // Ve mau nen den khi game over
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Mau den
        SDL_RenderClear(renderer);
        renderGameOverText(renderer, gameOverFont, board);
        // Hien thi chu "GAME OVER"
    }
    else {
        // Neu chua over, ve nen + cac doi tuong game binh thuong
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Xoa nen voi mau den
        SDL_RenderClear(renderer);
        board->Render(renderer);


        // Ve khung diem va level
        SDL_Rect infoBox;
        infoBox.x = BOARD_OFFSET_X + BOARD_WIDTH * CELL_SIZE + 50; // toa do x ben phai cua Board
        infoBox.y = 275; // vi tri y cua khung
        infoBox.w = 150; // Chieu rong cua khung
        infoBox.h = 100; // Chieu cao khung

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Màu khung - xam
        SDL_RenderDrawRect(renderer, &infoBox);


        // Hien thi diem + level ben trong khung
        SDL_Color textColor = { 255, 255, 255 }; // white
        int scoreX = infoBox.x + 10;
        int scoreY = infoBox.y + 10;
        int levelY = infoBox.y + 40;

        renderText(renderer, font, "Score: " + std::to_string(board->score), textColor, scoreX, scoreY);
        renderText(renderer, font, "Level: " + std::to_string(board->level), textColor, scoreX, levelY);

        // Hien thi so hang da pha trong level
        int linesClearedInLevel = board->linesCleared % 20;
        int linesNeededForNextLevel = 20;
        std::string linesClearedText = std::to_string(linesClearedInLevel) + "/" + std::to_string(linesNeededForNextLevel);
        int linesClearedY = infoBox.y + 70; // Vi tri Y (dieu chinh theo khung cua ban)

        if (board->level == 20) {
            linesClearedText = std::to_string(linesClearedInLevel) + "/20 (Max)";
        }

        renderText(renderer, font, linesClearedText, textColor, infoBox.x + 10, linesClearedY);
        // Ket thuc hien thi so hang da pha trong level

         // Hien thi thoi gian da qua
        int minutes = elapsedTime / 60;
        int seconds = elapsedTime % 60;

        std::stringstream timeStream;
        timeStream << "Time: " << std::setw(2) << std::setfill('0') << minutes
            << ":" << std::setw(2) << std::setfill('0') << seconds;
        std::string timeText = timeStream.str();

        int x = 300; // vi tri cua x 
        int y = 300; // vi tri cua y

        renderText(renderer, font, timeText, textColor, x, y);
    }

    SDL_RenderPresent(renderer);
}


void Game::Clean() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    std::cerr << "Game exited!" << std::endl;

}
