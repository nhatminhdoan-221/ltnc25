#include "SDL.h"
#include <iomanip>
#include <sstream> 
#include <iostream>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "game.h"
#include "board.h"
#include "GameUI.h"
#include "constants.h"

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
    currentGameState = MENU; 
    isSoundOn = true;
    menuBackgroundTexture = nullptr;
    currentMenuItemIndex = 0;
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

    //Khoi tao Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cerr << "SDL_mixer Initialized!" << std::endl;

    // Load Background music
    backgroundMusic = Mix_LoadMUS("tetristheme.mp3");
    if (!backgroundMusic) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    else {
        std::cerr << "Background music loaded successfully!" << std::endl;
    }

    if (backgroundMusic && isSoundOn) Mix_PlayMusic(backgroundMusic, -1);

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

    //Khoi tao menu
    menuBackgroundTexture = IMG_LoadTexture(renderer, "menuimage.png");
    if (!menuBackgroundTexture) {
        std::cerr << "Failed to load menu background: " << IMG_GetError() << std::endl;
    }
    else {
        std::cerr << "Menu background loaded successfully!" << std::endl;
    }
    // Khởi tạo các nút menu
    int buttonWidth = BUTTON_WIDTH;
    int buttonHeight = BUTTON_HEIGHT;
    int startX = (width - buttonWidth) / 2; // Canh giữa theo chiều ngang
    int startY = height / 3; // Vị trí Y ban đầu
    int offsetY = 40; // Khoảng cách giữa các nút theo chiều dọc

    // Nút "Start Game"
    MenuButton startGameButton;
    startGameButton.text = "Start Game";
    startGameButton.rect = { startX, startY, buttonWidth, buttonHeight };
    menuButtons.push_back(startGameButton);
    startY += offsetY; // Tăng Y cho nút tiếp theo

    // Nút "Quit"
    MenuButton quitButton;
    quitButton.text = "Quit";
    quitButton.rect = { startX, startY, buttonWidth, buttonHeight };
    menuButtons.push_back(quitButton);
    startY += offsetY; // Tăng Y cho nút tiếp theo

    // Nút "Sound: On/Off"
    MenuButton soundButton;
    soundButton.text = "Sound: On";
    soundButton.rect = { startX, startY, buttonWidth, buttonHeight };
    menuButtons.push_back(soundButton);

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

        if (currentGameState == MENU) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    currentMenuItemIndex--;
                    if (currentMenuItemIndex < 0) {
                        currentMenuItemIndex = menuButtons.size() - 1;
                    }
                    std::cout << "Selected menu item: " << currentMenuItemIndex << std::endl;
                    break;
                case SDLK_DOWN:
                    currentMenuItemIndex++;
                    if (currentMenuItemIndex >= menuButtons.size()) {
                        currentMenuItemIndex = 0;
                    }
                    std::cout << "Selected menu item: " << currentMenuItemIndex << std::endl;
                    break;
                case SDLK_RETURN: // Enter - Vao game
                case SDLK_SPACE:  
                    if (currentMenuItemIndex == 0) {
                        // => Chon Start Game
                        currentGameState = PLAYING;
                        board->Restart();
                        std::cout << "Start Game selected!" << std::endl;
                        startTime = SDL_GetTicks();
                        elapsedTime = 0;
                    }
                    else if (currentMenuItemIndex == 1) {
                        // => Chon Quit
                        isRunning = false;
                        std::cout << "Quit selected!" << std::endl;
                    }
                    else if (currentMenuItemIndex == 2) {
                        // "Sound: On/Off"
                        isSoundOn = !isSoundOn;
                        // Cap nhat trang thai am thanh
                        menuButtons[2].text = std::string("Sound: ") + (isSoundOn ? "On" : "Off");
                        std::cout << "Sound toggled to: " << (isSoundOn ? "On" : "Off") << std::endl;

                        if (isSoundOn) {
                            if (backgroundMusic) {
                                Mix_PlayMusic(backgroundMusic, -1);
                            }
                        }
                        else {
                            Mix_HaltMusic();
                        }
                    }
                    break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;

                for (size_t i = 0; i < menuButtons.size(); ++i) {
                    if (mouseX >= menuButtons[i].rect.x && mouseX < menuButtons[i].rect.x + menuButtons[i].rect.w &&
                        mouseY >= menuButtons[i].rect.y && mouseY < menuButtons[i].rect.y + menuButtons[i].rect.h) {
                        // Nut nao duoc click
                        if (i == 0) { // Start Game
                            currentGameState = PLAYING;
                            board->Restart();
                            std::cout << "Start Game clicked!" << std::endl;
                            startTime = SDL_GetTicks();
                            elapsedTime = 0;
                        }
                        else if (i == 1) { // QUIT
                            isRunning = false;
                            std::cout << "Quit clicked!" << std::endl;
                        }
                        else if (i == 2) { // SOUND
                            isSoundOn = !isSoundOn;
                            menuButtons[i].text = std::string("Sound: ") + (isSoundOn ? "On" : "Off");
                            std::cout << "Sound toggled to: " << (isSoundOn ? "On" : "Off") << std::endl;
                            if (isSoundOn) {
                                if (backgroundMusic) {
                                    Mix_PlayMusic(backgroundMusic, -1);
                                }
                            }
                            else {
                                Mix_HaltMusic();
                            }
                        }
                        break; // Thoat vong lap khi co nut da duoc click
                    }
                }
            }
        }
        else if (board->isGameOverState()) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;

                // Toa do cua Restart lay tu vi tri cua renderGameOverText
                int buttonWidth = BUTTON_WIDTH;
                int buttonHeight = BUTTON_HEIGHT;
                int centerX = SCREEN_WIDTH / 2;
                int startY = SCREEN_HEIGHT / 4;
                int offsetY = 50; // Dam bao gia tri giong voi trong renderGameOverText
                startY += offsetY * 2; // Sau "GAME OVER"
                startY += offsetY;     // Sau "Score:"
                startY += offsetY;     // Sau "Level:"
                startY += offsetY * 2; // Sau "Lines:"
                int restartButtonY = startY;
                int buttonSpacing = BUTTON_SPACING;
                int restartButtonX = centerX - buttonWidth / 2 - buttonSpacing - buttonWidth / 2;

                // Kiem tra nut chuot da click duoc chuan vao restart chua
                if (mouseX >= restartButtonX && mouseX < restartButtonX + buttonWidth &&
                    mouseY >= restartButtonY && mouseY < restartButtonY + buttonHeight) {
                    std::cout << "Restart button is clicked" << std::endl;
                    board->Restart(); // Goi ham restart cua Board
                    currentGameState = MENU; // Quay ve menu sau khi restart
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
                    currentGameState = MENU; // Quay ve menu sau khi restart
                }
            }
        }
        else {//CURRENTGAMESTATE=PLAYING
            // Cac phim bam trong game
            //artTime = SDL_GetTicks();
            //apsedTime = 0;
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

                case SDLK_m:
                    isSoundOn = !isSoundOn;
                    std::cout << "Music toggled in-game to: " << (isSoundOn ? "On" : "Off") << std::endl;
                    if (isSoundOn) {
                        if (backgroundMusic) {
                            Mix_PlayMusic(backgroundMusic, -1);
                        }
                    }
                    else {
                        Mix_HaltMusic();
                    }
                    break;
                }
            }
        }
    }
}

void Game::Update() {
    if (currentGameState == PLAYING) {
        Uint32 currentTime = SDL_GetTicks();
        elapsedTime = (currentTime - startTime) / 1000; // tinh thoi gian troi theo giay
        board->boardUpdate();
    }
    board->boardUpdate();
}



void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (currentGameState == MENU) {
        if (menuBackgroundTexture) {
            SDL_Rect destRect;
            destRect.x = 0;
            destRect.y = 0;
            destRect.w = width; // CHIEU RONG CUA CUA SO
            destRect.h = height; // CHIEU CAO CUA SO
            SDL_RenderCopy(renderer, menuBackgroundTexture, nullptr, &destRect);
        }
        else {
            // NEU KHONG CO HINH NEN -> MAU DEN 
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // BLACK
            SDL_RenderFillRect(renderer, nullptr);
        }

        // MAU CHU MENU
        SDL_Color textColor = { 255, 255, 255 }; // WHITE

        // MENU
        int centerX = width / 2; // VI TRI THEO PHUONG NGANG CUA MENU
        int startY = height / 3; // DO CAO MENU
        int offsetY = 50; // KHOANG CACH CAC MUC TRONG MENU

        // VE START GAME
        renderText(renderer, font, "Start Game", textColor, centerX - 100, startY);
        startY += offsetY;

        // VE QUIT
        renderText(renderer, font, "Quit", textColor, centerX - 100, startY);
        startY += offsetY;

        // SOUND: ON/OFF
        std::string soundText = "Sound: ";
        if (isSoundOn) {
            soundText += "On";
        }
        else {
            soundText += "Off";
        }
        renderText(renderer, font, soundText, textColor, centerX - 100, startY);
    }
    else if (board->isGameOverState()) {
        // Ve mau nen den khi game over
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // BLACK
        SDL_RenderClear(renderer);
        renderGameOverText(renderer, gameOverFont, board);
        // Hien thi chu "GAME OVER"
    }
    else { // currentGameState == PLAYING
        // Neu chua over, ve nen + cac doi tuong game binh thuong
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // = > NEN MAU DEN
        SDL_RenderClear(renderer);
        board->Render(renderer);


        // TOA DO KHUNG DIEM, LEVEL
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
    std::cerr << "Cleaning game..." << std::endl;

    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    if (menuBackgroundTexture) {
        SDL_DestroyTexture(menuBackgroundTexture);
        menuBackgroundTexture = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    std::cerr << "Game exited and cleaned up!" << std::endl;

}

