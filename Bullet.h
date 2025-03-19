#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include <SDL_ttf.h>
#include<ctime>
#include "Game.h"
#include "Slime.h"
#include "Boss.h"
#include "Obstacle.h"

struct BossBullet {
    int x, y, w, h;
    int speed;
    SDL_Texture* texture;  // Texture cho viên đạn

    BossBullet(int startX, int startY,int speed)
        : x(startX), y(startY), w(50), h(40), speed(speed) {

        // Tải hình ảnh cho viên đạn
        texture = IMG_LoadTexture(renderer, "img\\bullet\\bossbullet.png");  // Đường dẫn tới hình ảnh của viên đạn
        if (!texture) {
            std::cerr << "Failed to load boss bullet texture! SDL_image Error: " << IMG_GetError() << std::endl;
        }
    }

    void move() {
        x += speed;  // Đạn của boss di chuyển về phía Dino
    }

    void draw() {
        // Tạo vùng cắt (srcRect) nếu bạn muốn dùng sprite sheet, còn nếu chỉ có một hình ảnh thì không cần
        SDL_Rect destRect = { x, y, w, h };
        SDL_RenderCopy(renderer, texture, NULL, &destRect);  // Vẽ hình ảnh của viên đạn lên màn hình
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }

    bool outofscreen() {
        return x + w <= 0;
    }

    // Destructor để giải phóng texture
//    ~BossBullet() {
//        if (texture) {
//            SDL_DestroyTexture(texture);  // Giải phóng texture khi không còn sử dụng
//        }
//    }
};




struct Bullet {
    int x, y, w, h;
    int speed;
    SDL_Texture* texture;  // Texture cho đạn

    Bullet(int startX, int startY) : x(startX), y(startY), w(67), h(50), speed(5) {
        // Tải texture cho đạn
        texture = IMG_LoadTexture(renderer, "img\\bullet\\bullet.png");  // Đảm bảo đường dẫn đúng
        if (!texture) {
            std::cerr << "Failed to load bullet texture! SDL_image Error: " << IMG_GetError() << std::endl;
        }
    }

    void move() {
        x += speed;  // Đạn di chuyển sang phải
    }

    void draw() {
        // Vẽ đạn từ sprite sheet
        SDL_Rect bulletRect = { x, y, w, h };
        SDL_RenderCopy(renderer, texture, NULL, &bulletRect);  // Vẽ từ texture
    }

    bool checkCollisionWith(const Enemy& enemy) {
        return !(x + w <= enemy.x || x >= enemy.x + enemy.w || y + h <= enemy.y || y >= enemy.y + enemy.h);
    }

    bool checkCollisionWith(const Boss& boss) {
        return !(x + w <= boss.x || x >= boss.x + boss.w || y + h <= boss.y || y >= boss.y + boss.h);
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }

    bool checkCollisionWith(const BossBullet& bossbullet) {
        return !(x + w <= bossbullet.x || x >= bossbullet.x + bossbullet.w || y + h <= bossbullet.y || y >= bossbullet.y + bossbullet.h);
    }

    bool checkCollisionWith(const Obstacle& obstacle) {
        // Kiểm tra va chạm giữa Bullet và Obstacle
        return this->x < obstacle.x + obstacle.w &&
               this->x + this->w > obstacle.x &&
               this->y < obstacle.y + obstacle.h &&
               this->y + this->h > obstacle.y;
    }

    bool outofscreen(){
        return x > 800;  // Kiểm tra nếu đạn ra ngoài màn hình
    }

    // Destructor để giải phóng texture khi không sử dụng nữa
//    ~Bullet() {
//        if (texture) {
//            SDL_DestroyTexture(texture);  // Giải phóng texture khi không còn sử dụng
//        }
//    }
};


#endif // BULLET_H_INCLUDED
