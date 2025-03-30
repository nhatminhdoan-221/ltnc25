#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"

class Player : public GameObject {
public:
    Player(const char* textureSheet, int x, int y);
    void update() override;

private:
    int m_speed;
};

#endif // PLAYER_H
