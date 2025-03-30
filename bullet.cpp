#include "bullet.h"
#include "game.h"

Bullet::Bullet(const char* textureSheet, int x, int y) : GameObject(textureSheet, x, y), m_speed(-10) {}

void Bullet::update() {
    m_y += m_speed;
}
