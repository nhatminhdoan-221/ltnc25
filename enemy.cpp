
#include "enemy.h"
#include "game.h"

Enemy::Enemy(const char* textureSheet, int x, int y)
    : GameObject(textureSheet, x, y), m_moveDirection(1), m_moveSpeed(2), m_boundaryLeft(x - 50), m_boundaryRight(x + 50), m_movingRight(true) {}

void Enemy::update() {
    if (m_movingRight) {
        m_x += m_moveSpeed;
        if (m_x > m_boundaryRight) {
            m_movingRight = false;
        }
    } else {
        m_x -= m_moveSpeed;
        if (m_x < m_boundaryLeft) {
            m_movingRight = true;
        }
    }
}
