
#include "game_object.h"
#include "textue_manager.h"

GameObject::GameObject(const char* textureSheet, int x, int y) : m_x(x), m_y(y) {
    m_texture = TextureManager::LoadTexture(textureSheet);
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);
}

void GameObject::update() {}

void GameObject::render() {
    SDL_Rect srcRect = {0, 0, m_width, m_height};
    SDL_Rect destRect = {m_x, m_y, m_width, m_height};
    TextureManager::Draw(m_texture, srcRect, destRect);
}
