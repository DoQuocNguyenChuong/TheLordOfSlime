#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include <SDL_ttf.h>


SDL_Window* window=nullptr;
SDL_Renderer* renderer=nullptr;
int windowWidth=800;
int windowHeight=600;
TTF_Font* font;
const char* WINDOW_TITLE = "The Lord of Slime";

// Hàm khởi tạo SDL
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Mở font
    font = TTF_OpenFont("lazy.ttf", 24);  // Thay đường dẫn đúng đến file font của bạn
    if (!font) {
        std::cerr << "Font could not be loaded! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }


    return true;
}


// Hàm dọn dẹp SDL
void close() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void renderText(const std::string& text, int x, int y) {
    SDL_Color color = {0, 0, 0};  // Màu den
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Unable to create text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;  // Nếu không tạo được surface, thoát khỏi hàm
    }


    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // Giải phóng surface sau khi đã tạo texture
    if (!texture) {
        std::cerr << "Unable to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
        return;  // Nếu không tạo được texture, thoát khỏi hàm
    }

    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

    // dat vi tri van ban
    SDL_Rect textRect = {x - textWidth / 2, y, textWidth, textHeight};
    // Vẽ văn bản lên màn hình
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
    // Giải phóng tài nguyên texture
    SDL_DestroyTexture(texture);
}



#endif // GAME_H_INCLUDED
