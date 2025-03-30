#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>
#include <string>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* texture);
    static void Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect);
};

#endif // TEXTURE_MANAGER_H
