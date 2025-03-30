#include "player.h"
#include "game.h"

Player::Player(const char* textureSheet, int x, int y) : GameObject(textureSheet, x, y), m_speed(5) {}

void Player::update() {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_LEFT]) {
        m_x -= m_speed;
        if (m_x < 0) m_x = 0;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        m_x += m_speed;
        if (m_x > SCREEN_WIDTH - m_width) m_x = SCREEN_WIDTH - m_width;
    }
}
