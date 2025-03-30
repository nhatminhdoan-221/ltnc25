#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "texture_manager.h"
#include "game_object.h"
#include "player.h"
#include "enemy.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    bool running() const { return m_bRunning; }

    static SDL_Renderer* renderer;

private:
    SDL_Window* m_pWindow;
    bool m_bRunning;
    std::vector<GameObject*> m_gameObjects;
    Player* m_pPlayer;
    std::vector<Enemy*> m_enemies;
    std::vector<Bullet*> m_playerBullets;

    void createEnemies();
    void checkCollisions();
};

#endif // GAME_H
