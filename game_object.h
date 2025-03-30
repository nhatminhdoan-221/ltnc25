#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL.h>
#include "texture_manager.h"

class GameObject {
public:
    virtual ~GameObject() {}
    virtual void update() = 0;
    virtual void render() = 0;
    virtual int getX() const { return m_x; }
    virtual int getY() const { return m_y; }
    virtual int getWidth() const { return m_width; }
    virtual int getHeight() const { return m_height; }

protected:
    GameObject(const char* textureSheet, int x, int y);
    SDL_Texture* m_texture;
    int m_x, m_y;
    int m_width, m_height;
};

#endif // GAMEOBJECT_H
