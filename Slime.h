#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "Slime.h"

struct slime {
    int x, y, w, h;
    int velY, velX, velZ;  // Tốc độ rơi (trọng lực)
    bool isJumping, turnright, turnleft, isShooting;
    int health;  // Thanh máu
    int maxhealth;
    SDL_Texture* texture;  // Texture cho slime
    int currentFrameX;  // Khung hình hiện tại của slime (theo cột)
    int currentFrameY;  // Khung hình hiện tại của slime (theo hàng)
    int frameTimer;  // Bộ đếm thời gian để thay đổi khung hình
    int frameSpeed;  // Tốc độ thay đổi khung hình (thay đổi khung hình mỗi 2 lần cập nhật)

    slime(int health)
         : x(100), y(470), w(80), h(80), velY(0), isJumping(false),isShooting(false), health(health),maxhealth(health), currentFrameX(0), currentFrameY(0), frameTimer(0), frameSpeed(2) {
        // Tải spritesheet cho slime
        texture = IMG_LoadTexture(renderer, "img\\slime\\slime.png");  // Đảm bảo đường dẫn đúng
        if (!texture) {
            std::cerr << "Failed to load slime texture! SDL_image Error: " << IMG_GetError() << std::endl;
        }
    }

    void move() {
        // Di chuyển theo trục Y (nhảy)
        if (isJumping) {
            y += velY;
            velY += 1;  // Gia tốc trọng lực

            // Nếu slime đã rơi xuống mặt đất, dừng lại
            if (y >= 470) {
                y = 470;  // Đảm bảo slime không xuyên qua mặt đất
                isJumping = false;  // Dừng nhảy
                velY = 0;  // Dừng chuyển động dọc
                currentFrameY = 2;
            }

            // Khi nhảy, sử dụng hàng đầu tiên (currentFrameY = 0)

            if (!isJumping) {
            currentFrameY = 0;
          }
        }

        // Di chuyển sang phải
        if (turnright) {
            x += 5;  // Di chuyển sang phải với tốc độ cố định
            currentFrameY = 2;  // Cập nhật sprite để di chuyển sang phải
            if (x >= 600) {
                x = 600;  // Dừng lại khi đến giới hạn
                turnright = false;  // Tắt chế độ di chuyển sang phải
            }
        }

        // Di chuyển sang trái
        if (turnleft) {
            x -= 5;  // Di chuyển sang trái với tốc độ cố định
            currentFrameY = 2;  // Cập nhật sprite để di chuyển sang trái
            if (x <= 0) {
                x = 0;  // Dừng lại khi đến giới hạn
                turnleft = false;  // Tắt chế độ di chuyển sang trái
            }
        }

        // Cập nhật trạng thái khi slime bắn đạn
        if (isShooting) {
            currentFrameY = 2;  // Cập nhật sprite để bắn (hoặc có thể tùy chỉnh thêm)
        }

        // Cập nhật khung hình
        if (frameTimer >= frameSpeed) {
            currentFrameX++;  // Chuyển sang khung hình tiếp theo
            if (currentFrameX >= 9) {  // Kiểm tra nếu đã hết 9 khung hình
                currentFrameX = 0;  // Quay lại khung hình đầu tiên
            }
            frameTimer = 0;  // Đặt lại bộ đếm thời gian
        } else {
            frameTimer++;  // Tăng bộ đếm thời gian
        }
    }

    void jump() {
        if (!isJumping) {
            velY = -20;  // Tốc độ nhảy ban đầu
            isJumping = true;
            currentFrameY = 0;  // Lấy hàng đầu tiên khi nhảy
            onJump();
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

    void shoot() {
        isShooting = true;  // Bật chế độ bắn
    }

    void stopShoot() {
        isShooting = false;  // Dừng chế độ bắn
    }

    void draw() {
        // Tính toán chiều rộng và chiều cao của mỗi khung hình
        int frameWidth = 825 / 9;  // Chiều rộng mỗi khung hình (825 px / 9 cột)
        int frameHeight = 283 / 3;  // Chiều cao mỗi khung hình (283 px / 3 hàng)

        // Tạo vùng cắt từ spritesheet (tính toán cột và hàng dựa trên vị trí)
        SDL_Rect srcRect = { currentFrameX * frameWidth, currentFrameY * frameHeight, frameWidth, frameHeight };
        SDL_Rect destRect = { x, y, w, h };

        // Vẽ slime từ spritesheet
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }

    void drawHealthBar() {
    // Vẽ viền cho thanh máu
    SDL_Rect borderRect = { 600 - 2, 0 - 2, 200 + 4, 50 + 4 };  // Viền xung quanh thanh máu
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Màu trắng cho viền
    SDL_RenderFillRect(renderer, &borderRect);  // Vẽ viền ngoài

    // Tính toán tỷ lệ phần trăm máu còn lại
    float healthPercentage = (float)health / maxhealth;

    // Chọn màu sắc thanh máu tùy vào tỷ lệ máu còn lại
    SDL_Color healthColor;
    if (healthPercentage > 0.6f) {
        healthColor = { 0, 255, 0, 255 };  // Màu xanh lá cây khi máu còn nhiều
    } else if (healthPercentage > 0.3f) {
        healthColor = { 255, 255, 0, 255 };  // Màu vàng khi máu còn ít
    } else {
        healthColor = { 255, 0, 0, 255 };  // Màu đỏ khi máu rất ít
    }

    // Vẽ thanh máu
    SDL_Rect healthBarRect = { 600, 0, (int)(200 * healthPercentage), 50 };
    SDL_SetRenderDrawColor(renderer, healthColor.r, healthColor.g, healthColor.b, healthColor.a);  // Màu thanh máu
    SDL_RenderFillRect(renderer, &healthBarRect);  // Vẽ thanh máu

    // Vẽ số máu còn lại lên thanh máu
    std::string healthText = std::to_string(health) + " / " + std::to_string(maxhealth);
    renderText(healthText, 600 + 100, 25);  // Hiển thị số máu ở giữa thanh máu
}
    // Destructor để giải phóng texture
//    ~slime() {
//        if (texture) {
//            SDL_DestroyTexture(texture);  // Giải phóng texture khi không còn sử dụng
//        }
//    }
};

#endif // SLIME_H_INCLUDED
