#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include "Game.h"
#include "Slime.h"




struct slime{
    int x, y, w, h;
    int velY,velX,velZ;  // Tốc độ rơi (trọng lực)
    bool isJumping,turnright,turnleft;
    int health;  // Thanh máu

    slime() : x(100), y(400), w(50), h(50), velY(0), isJumping(false), health(10) {}

    void move() {
    // Di chuyển theo trục Y (nhảy)
    if (isJumping) {
        y += velY;
        velY += 1;  // Gia tốc trọng lực

        // Nếu slime đã rơi xuống mặt đất, dừng lại
        if (y >= 400) {
            y = 400;  // Đảm bảo Dino không xuyên qua mặt đất
            isJumping = false;  // Dừng nhảy
            velY = 0;  // Dừng chuyển động dọc
        }
    }

     //Di chuyển sang phải
    if (turnright) {
        x += 5;  // Di chuyển sang phải với tốc độ cố định
        if (x >= 600) {
            x = 600;  // Dừng lại khi đến giới hạn
            turnright = false;  // Tắt chế độ di chuyển sang phải
        }
    }

    // Di chuyển sang trái
    if (turnleft) {
        x -= 5;  // Di chuyển sang trái với tốc độ cố định
        if (x <= 0) {
            x = 0;  // Dừng lại khi đến giới hạn
            turnleft = false;  // Tắt chế độ di chuyển sang trái
        }
    }
}

void jump() {
    if (!isJumping) {
        velY = -20;  // Tốc độ nhảy ban đầu
        isJumping = true;
    }
}

void left() {
    turnleft = true;  // Bật chế độ di chuyển sang trái
}

void right() {
    turnright = true;  // Bật chế độ di chuyển sang phải
}

void stopLeft() {
    turnleft = false;  // Dừng di chuyển sang trái
}

void stopRight() {
    turnright = false;  // Dừng di chuyển sang phải
 }
 void draw() {
        SDL_Rect slimerect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Màu den cho slime
        SDL_RenderFillRect(renderer, &slimerect);
    }

void drawHealthBar() {
    SDL_Rect healthBar = {600, 0,200, 50};  // Thanh máu ở góc phải
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu xanh cho thanh máu
    SDL_RenderFillRect(renderer, &healthBar);  // Vẽ thanh máu


    // Vẽ thanh máu còn lại
   healthBar.w = (200 * health) / 10;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Màu do cho phần máu còn lại
    SDL_RenderFillRect(renderer, &healthBar);  // Vẽ thanh máu còn lại
}

};
#endif // SLIME_H_INCLUDED
