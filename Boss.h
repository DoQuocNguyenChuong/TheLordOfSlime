#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include <SDL_ttf.h>
#include "Game.h"

struct Boss {
    int x, y, w, h;
    int health;
    int speed;

    Boss() : x(windowWidth - 150), y(200), w(100), h(100), health(100), speed(2) {}

    void move(const slime& slime) {
        // Boss di chuyển giống như kẻ địch bay, nhưng có thể di chuyển chậm hơn
        if (y < slime.y) y += speed;
        else if (y > slime.y) y -= speed;
    }

    void draw() {
        SDL_Rect bossRect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);  // Màu hồng cho Boss
        SDL_RenderFillRect(renderer, &bossRect);
    }

    void drawHealthBar() {
    SDL_Rect healthBar = { x, y - 10, w, 5 };  // Thanh máu của boss sẽ nằm trên đầu boss
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ cho thanh máu
    SDL_RenderFillRect(renderer, &healthBar);

    // Vẽ thanh máu còn lại
    healthBar.w = (w * health) / 100;  // Tỉ lệ phần trăm máu của boss
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Màu xanh cho máu còn lại
    SDL_RenderFillRect(renderer, &healthBar);
}

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }
};


#endif // BOSS_H_INCLUDED
