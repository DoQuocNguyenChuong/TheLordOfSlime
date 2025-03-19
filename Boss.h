#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include "Game.h"

struct Boss {
    int x, y, w, h;
    int health;
    int maxHealth;   // Máu tối đa của boss
    int speed;
    SDL_Texture* texture;  // Texture cho Boss từ sprite sheet
    int currentFrame;       // Khung hình hiện tại
    int frameWidth;         // Kích thước khung hình theo chiều ngang (240px)
    int frameHeight;        // Kích thước khung hình theo chiều dọc (300px)
    int numCols;            // Số cột trong sprite sheet
    int numRows;            // Số hàng trong sprite sheet
    bool isShooting=true;        // Cờ kiểm tra xem Boss có đang bắn đạn không
    int animationSpeed;     // Tốc độ thay đổi khung hình

    Boss(int bosshealth,int speed)
        : x(windowWidth - 150), y(200), w(100), h(100), health(bosshealth),maxHealth(bosshealth), speed(speed), currentFrame(0),
          frameWidth(300), frameHeight(300), numCols(4), numRows(4), isShooting(false),animationSpeed(1) {

        texture = IMG_LoadTexture(renderer, "img\\enemy\\fireelementboss.png");  // Tải sprite sheet
        if (!texture) {
            std::cerr << "Failed to load boss sprite sheet! SDL_image Error: " << IMG_GetError() << std::endl;
        }
    }

    void move(const slime& slime) {
        // Boss di chuyển giống như kẻ địch bay, nhưng có thể di chuyển chậm hơn
        if (y < slime.y) y += speed;
        else if (y > slime.y) y -= speed;
    }

    void updateAnimation() {
        if (isShooting) {
            // Nếu Boss đang bắn đạn, chỉ cập nhật khung hình cuối (khung hình 1 hoặc 2)
            if (currentFrame < (numCols * numRows) - 1) {
                currentFrame += animationSpeed;
            } else {
                currentFrame = (numCols * numRows) - 1;  // Giữ khung hình cuối cùng
            }
        } else {
            currentFrame = 0;  // Giữ khung hình đầu tiên nếu không bắn đạn
        }
    }

    void draw() {
        updateAnimation();  // Chỉ cập nhật khung hình khi bắn đạn

        // Tính chỉ số dòng và cột của khung hình
         int row = currentFrame / numCols;  // Tính chỉ số dòng từ khung hình
        int col = currentFrame % numCols;  // Tính chỉ số cột từ khung hình

        // Tạo vùng cắt (srcRect) từ sprite sheet
        SDL_Rect srcRect = { col * frameWidth, row * frameHeight, frameWidth, frameHeight };
        SDL_Rect destRect = { x, y, w, h };

        // Vẽ Boss từ sprite sheet
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }

    void drawHealthBar() {
        SDL_Rect healthBar = { x, y - 10, w, 10 };  // Thanh máu của boss sẽ nằm trên đầu boss
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ cho thanh máu
        SDL_RenderFillRect(renderer, &healthBar);

        // Vẽ thanh máu còn lại
        healthBar.w = (w * health) / maxHealth;  // Tỉ lệ phần trăm máu của boss
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Màu xanh cho máu còn lại
        SDL_RenderFillRect(renderer, &healthBar);
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }

    // Gọi hàm này khi Boss bắn đạn
    void startShooting() {
        isShooting = true;  // Bật cờ đang bắn đạn
    }

    // Gọi hàm này khi Boss không còn bắn đạn
    void stopShooting() {
        isShooting = false;  // Tắt cờ không bắn đạn
    }

//    ~Boss() {
//        if (texture) {
//            SDL_DestroyTexture(texture);  // Giải phóng texture khi không sử dụng nữa
//        }
//    }
};

#endif // BOSS_H_INCLUDED
