#ifndef BULLET_H
#define BULLET_H

#include "game_object.h"

class Bullet : public GameObject {
public:
    Bullet(const char* textureSheet, int x, int y);
    void update() override;

private:
    int m_speed;
};

#endif // BULLET_H
