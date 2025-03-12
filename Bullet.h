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

    BossBullet(int startX, int startY) : x(startX), y(startY), w(10), h(5), speed(-5) {}

    void move() {
        x += speed;  // Đạn của boss di chuyển về phía Dino
    }

    void draw() {
        SDL_Rect bulletRect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);  // Màu cam cho đạn của boss
        SDL_RenderFillRect(renderer, &bulletRect);
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }

    bool outofscreen(){
         return !(x>=0);
    }

};



struct Bullet {
    int x, y, w, h;
    int speed;

    Bullet(int startX, int startY) : x(startX), y(startY), w(10), h(5), speed(5) {}

    void move() {
        x += speed;  // Đạn di chuyển sang phải
    }

    void draw() {
        SDL_Rect bulletRect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Màu xanh dương cho đạn
        SDL_RenderFillRect(renderer, &bulletRect);
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

    bool checkCollisionWith(const Obstacle& obstacle) {
    // Kiểm tra va chạm giữa Bullet và Obstacle
    // Giả sử Bullet có vị trí (x, y) và kích thước (width, height)
    return this->x < obstacle.x + obstacle.w &&
           this->x + this->w > obstacle.x &&
           this->y < obstacle.y + obstacle.h &&
           this->y + this->h > obstacle.y;
}

    bool outofscreen(){
         return !(x<=800);
    }
};


#endif // BULLET_H_INCLUDED
