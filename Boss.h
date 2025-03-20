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
    // Vị trí của thanh máu boss dưới thanh máu slime
    int offsetY = 100;  // Khoảng cách từ thanh máu slime đến thanh máu boss (có thể thay đổi)

    // Vẽ viền cho thanh máu boss
    SDL_Rect borderRect = { 600 - 5, offsetY - 5, 200 + 10, 50 + 10 };  // Viền xung quanh thanh máu boss
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Màu trắng cho viền
    SDL_RenderFillRect(renderer, &borderRect);  // Vẽ viền ngoài

    // Tính toán tỷ lệ phần trăm máu còn lại của boss
    float healthPercentage = (float)health / maxHealth;  // Sử dụng maxHealth thay vì maxhealth

    // Chọn màu sắc thanh máu boss tùy vào tỷ lệ máu còn lại
    SDL_Color healthColor;
    if (healthPercentage > 0.6f) {
        healthColor = { 0, 255, 0, 255 };  // Màu xanh lá cây khi máu còn nhiều
    } else if (healthPercentage > 0.3f) {
        healthColor = { 255, 255, 0, 255 };  // Màu vàng khi máu còn ít
    } else {
        healthColor = { 255, 0, 0, 255 };  // Màu đỏ khi máu rất ít
    }

    // Vẽ thanh máu boss
    SDL_Rect healthBarRect = { 600, offsetY, (int)(200 * healthPercentage), 50 };
    SDL_SetRenderDrawColor(renderer, healthColor.r, healthColor.g, healthColor.b, healthColor.a);  // Màu thanh máu
    SDL_RenderFillRect(renderer, &healthBarRect);  // Vẽ thanh máu

    // Vẽ số máu còn lại lên thanh máu của boss
    std::string healthText = std::to_string(health) + " / " + std::to_string(maxHealth);  // Hiển thị số máu
    renderText(healthText, 600 + 100, offsetY + 25);  // Hiển thị số máu ở giữa thanh máu boss

    // Option: Vẽ thêm hiệu ứng nhấp nháy khi máu của boss dưới 10%
    if (healthPercentage < 0.1f) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ nhấp nháy khi máu thấp
        SDL_RenderDrawRect(renderer, &healthBarRect);  // Vẽ viền nhấp nháy cho thanh máu
    }

    // Vẽ chữ "BOSS" trên thanh máu
    std::string bossText = "BOSS";
    renderText(bossText, 600 + 10, 60);  // Hiển thị chữ "BOSS" ở góc trái thanh máu
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
