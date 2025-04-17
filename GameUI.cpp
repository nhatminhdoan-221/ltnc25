#include "board.h"
#include "GameUI.h"
#include "constants.h"


void renderGameOverText(SDL_Renderer* renderer, TTF_Font* font, Board* board) {
    SDL_Color textColor = { 255, 255, 255 }; // MAU TRANG
    SDL_Color buttonColor = { 150, 150, 150 }; // Mau nen nut
    int centerX = SCREEN_WIDTH / 2;
    int startY = SCREEN_HEIGHT / 4;
    int offsetY = 50; // Khoang cach giua cac dong
    int buttonWidth = BUTTON_WIDTH;
    int buttonHeight = BUTTON_HEIGHT;
    int buttonSpacing = BUTTON_SPACING;

    // Hien thi "GAME OVER"
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "GAME OVER", textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Rect gameOverRect;
    gameOverRect.w = gameOverSurface->w;
    gameOverRect.h = gameOverSurface->h;
    gameOverRect.x = centerX - gameOverRect.w / 2;
    gameOverRect.y = startY;
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);

    startY += offsetY * 2;

    // hien thi diem so
    std::string scoreText = "Score: " + std::to_string(board->score);
    renderText(renderer, font, scoreText, textColor, centerX - 100, startY);
    startY += offsetY;

    // hien thi level
    std::string levelText = "Level: " + std::to_string(board->level);
    renderText(renderer, font, levelText, textColor, centerX - 100, startY);
    startY += offsetY;

    // hien thi so dong da pha duoc
    std::string linesText = "Lines: " + std::to_string(board->linesCleared);
    renderText(renderer, font, linesText, textColor, centerX - 100, startY);
    startY += offsetY * 2;

    // Phim restart
    SDL_Rect restartButtonRect;
    restartButtonRect.w = buttonWidth;
    restartButtonRect.h = buttonHeight;
    restartButtonRect.x = centerX - buttonWidth / 2 - buttonSpacing - buttonWidth / 2; // ben trai
    restartButtonRect.y = startY;
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &restartButtonRect);

    // Chu Restart tren nut
    std::string restartText = "Restart";
    SDL_Surface* restartSurface = TTF_RenderText_Solid(font, restartText.c_str(), textColor);
    SDL_Texture* restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
    SDL_Rect restartTextRect;
    restartTextRect.w = restartSurface->w;
    restartTextRect.h = restartSurface->h;
    restartTextRect.x = restartButtonRect.x + (restartButtonRect.w - restartTextRect.w) / 2;
    restartTextRect.y = restartButtonRect.y + (restartButtonRect.h - restartTextRect.h) / 2;
    SDL_RenderCopy(renderer, restartTexture, nullptr, &restartTextRect);
    SDL_FreeSurface(restartSurface);
    SDL_DestroyTexture(restartTexture);

    // Nut QUIT
    SDL_Rect homeButtonRect;
    homeButtonRect.w = buttonWidth;
    homeButtonRect.h = buttonHeight;
    homeButtonRect.x = centerX + buttonSpacing + buttonWidth / 2 - buttonWidth / 2; // Nam ben phai
    homeButtonRect.y = startY;
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &homeButtonRect);

    // Chu "Quit" tren nut
    std::string homeText = "Quit";
    SDL_Surface* homeSurface = TTF_RenderText_Solid(font, homeText.c_str(), textColor);
    SDL_Texture* homeTexture = SDL_CreateTextureFromSurface(renderer, homeSurface);
    SDL_Rect homeTextRect;
    homeTextRect.w = homeSurface->w;
    homeTextRect.h = homeSurface->h;
    homeTextRect.x = homeButtonRect.x + (homeButtonRect.w - homeTextRect.w) / 2;
    homeTextRect.y = homeButtonRect.y + (homeButtonRect.h - homeTextRect.h) / 2;
    SDL_RenderCopy(renderer, homeTexture, nullptr, &homeTextRect);
    SDL_FreeSurface(homeSurface);
    SDL_DestroyTexture(homeTexture);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface error: %s", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface error: %s", SDL_GetError());
    }

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}