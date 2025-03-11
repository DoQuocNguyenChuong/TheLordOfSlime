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

    Obstacle(int startX) : x(startX), y(400), w(50), h(50), speed(5) {}

    void move() {
        x -= speed;
        if (x < -w) {
            x = windowWidth;
        }
    }

    void draw() {
        SDL_Rect obstacleRect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ cho Obstacle
        SDL_RenderFillRect(renderer, &obstacleRect);
    }

    bool checkCollisionWith(const slime& slime) {
        return !(x + w <= slime.x || x >= slime.x + slime.w || y + h <= slime.y || y >= slime.y + slime.h);
    }
};

#endif // OBSTACLE_H_INCLUDED
