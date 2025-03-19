#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Game.h"

struct Enemy {
    int x, y, w, h;
    int health;
    int speed;
    SDL_Texture* spriteSheet;  // Texture chứa toàn bộ sprite sheet
    int currentFrame;  // Khung hình hiện tại
    int frameWidth, frameHeight;  // Kích thước mỗi khung hình trong sprite sheet
    int numCols, numRows;  // Số cột và số dòng trong sprite sheet
    int frameDelay;  // Thời gian delay giữa mỗi khung hình
    int frameTimer;  // Bộ đếm thời gian để quản lý frame delay

    Enemy(int startX, int startY)
        : x(startX), y(startY), w(100), h(100), health(1), speed(3),
          currentFrame(0), frameWidth(150), frameHeight(150),
          numCols(4), numRows(3), frameDelay(16), frameTimer(0) {

        spriteSheet = IMG_LoadTexture(renderer, "img\\fireelement.png");

        if (!spriteSheet) {
            std::cerr << "Không thể tải sprite sheet của Enemy! SDL_image Error: " << IMG_GetError() << std::endl;
        }
    }

    void move(const slime& slime) {
        // Kẻ địch di chuyển về phía slime (di chuyển theo trục X và Y)
        if (x < slime.x) x += speed;
        else if (x > slime.x) x -= speed;

        if (y < slime.y) y += speed;
        else if (y > slime.y) y -= speed;
    }

    void updateAnimation() {
        frameTimer += 1;  // Tăng bộ đếm thời gian mỗi vòng lặp
        if (frameTimer >= frameDelay) {
            frameTimer = 0;  // Reset bộ đếm thời gian
            currentFrame = (currentFrame + 1) % (numCols * numRows);  // Chuyển sang frame tiếp theo
        }
    }

    void draw() {
        updateAnimation();  // Cập nhật khung hình hiện tại

        // Tính toán vị trí cắt (srcRect) trong sprite sheet
        int row = currentFrame / numCols;  // Tính chỉ số dòng
        int col = currentFrame % numCols;  // Tính chỉ số cột
        SDL_Rect srcRect = { col * frameWidth, row * frameHeight, frameWidth, frameHeight };
        SDL_Rect destRect = { x, y, w, h };

        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);  // Vẽ frame hiện tại lên màn hình
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }
};

#endif // ENEMY_H_INCLUDED
