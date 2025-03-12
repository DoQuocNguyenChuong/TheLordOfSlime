#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include "Game.h"


struct Enemy {
    int x, y, w, h;
    int health;
    int speed;

    Enemy(int startX, int startY) : x(startX), y(startY), w(50), h(50), health(1), speed(3) {}

    void move(const slime& slime){
        // Kẻ địch bay (di chuyển theo trục Y)
        if (x < slime.x) x += speed;
        else if (x > slime.x) x -= speed;

        if (y < slime.y) y += speed;
        else if (y > slime.y) y -= speed;
//        x -= speed;
//        if (x < -w) {
//            x = windowWidth;
//        }
   }

    void draw() {
        SDL_Rect enemyRect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ cho Enemy
        SDL_RenderFillRect(renderer, &enemyRect);
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }
};



#endif // ENEMY_H_INCLUDED
