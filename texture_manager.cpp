#include "texture_manager.h"
#include "game.h"
#include <SDL_image.h>

SDL_Texture* TextureManager::LoadTexture(const char* texture) {
    SDL_Surface* tmpSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return tex;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect) {
    SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);
}
