#ifndef OBSTACLE_H_INCLUDED
#define OBSTACLE_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include "Game.h"


struct Obstacle {
    int x, y, w, h;
    int speed;
    SDL_Texture* spriteSheet;  // Thêm spriteSheet cho Obstacle
    int currentFrame;  // Khung hình hiện tại
    int frameWidth, frameHeight;  // Kích thước mỗi khung hình trong sprite sheet
    int numCols, numRows;  // Số cột và số dòng trong sprite sheet
    int frameDelay;  // Thời gian delay giữa mỗi khung hình
    int frameTimer;  // Bộ đếm thời gian để quản lý frame delay

    Obstacle(int startX, int startY,int speed)
        : x(startX), y(startY), w(100), h(100), speed(speed),
          currentFrame(0), frameWidth(150), frameHeight(150),
          numCols(4), numRows(4), frameDelay(16), frameTimer(0) {

    spriteSheet = IMG_LoadTexture(renderer, "img\\enemy\\earthelement.png");  // Tải hình ảnh chướng ngại vật
        if (!spriteSheet) {
            std::cerr << "Failed to load earthelement image! SDL_image Error: " << IMG_GetError() << std::endl;
        }
    }

    void move() {
        x -= speed;
        if (x < -w) {
            x = windowWidth;
            currentFrame = 0;  // Đặt lại frame animation về frame đầu tiên
        }
    }

    void updateAnimation() {
        frameTimer += 1;  // Tăng bộ đếm thời gian mỗi vòng lặp
        if (frameTimer >= frameDelay) {
            frameTimer = 0;  // Reset bộ đếm thời gian
            currentFrame = (currentFrame + 1) % (numCols * numRows);  // Chuyển sang frame tiếp theo, quay lại khi hết frame
        }
    }

    void draw() {
        updateAnimation();

       // Tính toán vị trí cắt (srcRect) trong sprite sheet
        int row = currentFrame / numCols;  // Tính chỉ số dòng
        int col = currentFrame % numCols;  // Tính chỉ số cột
        SDL_Rect srcRect = { col * frameWidth, (row) * frameHeight, frameWidth, frameHeight };
        SDL_Rect destRect = { x, 450, w, h };

        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);  // Vẽ frame hiện tại lên màn hình
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }
    bool outofscreen(){
         return !(x>=0);
    }

//     ~Obstacle() {
//        if (texture) {
//            SDL_DestroyTexture(texture);  // Giải phóng texture khi không sử dụng nữa
//        }
//    }
};

#endif // OBSTACLE_H_INCLUDED
