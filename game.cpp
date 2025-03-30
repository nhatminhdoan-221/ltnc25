#include "game.h"
#include "texture_manager.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include <iostream>

SDL_Renderer* Game::renderer = nullptr;

Game::Game() : m_pWindow(nullptr), m_bRunning(false), m_pPlayer(nullptr) {}

Game::~Game() {
    for (auto& gameObject : m_gameObjects) {
        delete gameObject;
    }
    m_gameObjects.clear();

    for (auto& enemy : m_enemies) {
        delete enemy;
    }
    m_enemies.clear();

    for (auto& bullet : m_playerBullets) {
        delete bullet;
    }
    m_playerBullets.clear();

    delete m_pPlayer;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL initialized!" << std::endl;
        m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (m_pWindow) {
            std::cout << "Window created!" << std::endl;
            renderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
            if (renderer) {
                std::cout << "Renderer created!" << std::endl;
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                m_bRunning = true;

                // Tạo Player
                m_pPlayer = new Player("images/player.png", 100, 400);
                m_gameObjects.push_back(m_pPlayer);

                // Tạo Enemies
                createEnemies();
            } else {
                std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                return false;
            }
        } else {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
    } else {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void Game::createEnemies() {
    int enemyStartX = 100;
    int enemyStartY = 50;
    int enemySpacingX = 50;
    int enemySpacingY = 40;
    int numRows = 3;
    int numCols = 5;

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            Enemy* enemy = new Enemy("images/enemy.png", enemyStartX + j * enemySpacingX, enemyStartY + i * enemySpacingY);
            m_enemies.push_back(enemy);
            m_gameObjects.push_back(enemy);
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_bRunning = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                Bullet* bullet = new Bullet("images/bullet.png", m_pPlayer->getX() + 16, m_pPlayer->getY());
                m_playerBullets.push_back(bullet);
                m_gameObjects.push_back(bullet);
            }
        }
    }
}

void Game::update() {
    for (auto& gameObject : m_gameObjects) {
        gameObject->update();
    }

    // Cập nhật vị trí các viên đạn
    for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); ) {
        (*it)->update();
        if ((*it)->getY() < 0) {
            delete *it;
            it = m_playerBullets.erase(it);
        } else {
            ++it;
        }
    }

    checkCollisions();
}

void Game::checkCollisions() {
    for (auto itBullet = m_playerBullets.begin(); itBullet != m_playerBullets.end(); ) {
        bool collision = false;
        for (auto itEnemy = m_enemies.begin(); itEnemy != m_enemies.end(); ) {
            if ((*itBullet)->getX() < (*itEnemy)->getX() + (*itEnemy)->getWidth() &&
                (*itBullet)->getX() + (*itBullet)->getWidth() > (*itEnemy)->getX() &&
                (*itBullet)->getY() < (*itEnemy)->getY() + (*itEnemy)->getHeight() &&
                (*itBullet)->getY() + (*itBullet)->getHeight() > (*itEnemy)->getY()) {
                std::cout << "Enemy hit!" << std::endl;
                delete *itEnemy;
                itEnemy = m_enemies.erase(itEnemy);
                for (auto itGameObject = m_gameObjects.begin(); itGameObject != m_gameObjects.end(); ++itGameObject) {
                    if (*itGameObject == *itEnemy) {
                        m_gameObjects.erase(itGameObject);
                        break;
                    }
                }
                delete *itBullet;
                itBullet = m_playerBullets.erase(itBullet);
                collision = true;
                break;
            } else {
                ++itEnemy;
            }
        }
        if (!collision) {
            ++itBullet;
        }
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    for (const auto& gameObject : m_gameObjects) {
        gameObject->render();
    }
    SDL_RenderPresent(renderer);
}
