#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"

class Enemy : public GameObject {
public:
    Enemy(const char* textureSheet, int x, int y);
    void update() override;

private:
    int m_moveDirection;
    int m_moveSpeed;
    int m_boundaryLeft;
    int m_boundaryRight;
    bool m_movingRight;
};

#endif // ENEMY_H
