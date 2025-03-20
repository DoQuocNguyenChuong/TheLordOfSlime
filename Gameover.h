#ifndef GAMEOVER_H_INCLUDED
#define GAMEOVER_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include<string>
#include "Menu.h" // Thêm file menu vào đây để có thể quay lại menu

// Hàm để hiển thị màn hình kết thúc game
bool showGameOverScreen(SDL_Renderer* renderer, int windowWidth, int windowHeight, bool playerWon) {
    SDL_Event e;
    bool restart = false;
    bool quit = false;

    // Load background image for game over screen
    SDL_Texture* gameOverBackground = loadTexture(playerWon ? "img\\background\\Victory.jpeg" : "img\\background\\Defeat.jpeg", renderer);
    if (!gameOverBackground) {
        std::cerr << "Failed to load game over screen background!" << std::endl;
        return false;
    }

    // Load font cho thông báo và nút restart
    TTF_Font* font = TTF_OpenFont("VNI-Nhatban.ttf", 50); // Đường dẫn tới font
    if (!font) {
        std::cerr << "Failed to load font!" << std::endl;
        return false;
    }

    // Màu sắc cho các thông báo
    SDL_Color textColor = {255, 255, 0};  // Màu vang

    // Tạo texture cho thông báo "Bạn thắng" hoặc "Bạn thua"
    SDL_Surface* messageSurface = TTF_RenderText_Solid(font, playerWon ? "YOU WIN!" : "YOU LOSE!", textColor);
    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_FreeSurface(messageSurface);

    // Tạo texture cho nút Restart
    SDL_Surface* restartSurface = TTF_RenderText_Solid(font, "Restart", textColor);
    SDL_Texture* restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
    SDL_FreeSurface(restartSurface);

    // Tạo texture cho nút Exit
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Exit", textColor);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_FreeSurface(exitSurface);

    // Lấy kích thước của thông báo và các nút
    int messageWidth, messageHeight, restartWidth, restartHeight, exitWidth, exitHeight;
    SDL_QueryTexture(messageTexture, NULL, NULL, &messageWidth, &messageHeight);
    SDL_QueryTexture(restartTexture, NULL, NULL, &restartWidth, &restartHeight);
    SDL_QueryTexture(exitTexture, NULL, NULL, &exitWidth, &exitHeight);

   // Đặt vị trí cho thông báo và các nút
    SDL_Rect messageRect = {windowWidth / 2 - messageWidth / 2, windowHeight / 3 - messageHeight / 2, messageWidth, messageHeight};
    SDL_Rect restartButtonRect = {windowWidth / 2 - restartWidth / 2, windowHeight / 2 + 50, restartWidth, restartHeight};
    SDL_Rect exitButtonRect = {windowWidth / 2 - exitWidth / 2, windowHeight / 2 + 150, exitWidth, exitHeight};  // Vị trí Exit

    // Màn hình game over luôn hiển thị cho đến khi người chơi nhấn nút restart
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Handle mouse events (nút restart)
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {

                    onClick();

                    int x = e.button.x;
                    int y = e.button.y;

                    // Check if the user clicked on the restart button
                    if (x >= restartButtonRect.x && x <= restartButtonRect.x + restartButtonRect.w &&
                        y >= restartButtonRect.y && y <= restartButtonRect.y + restartButtonRect.h) {
                        restart = true;
                        quit=true;
                    }
                    // Kiểm tra nếu người dùng click vào nút exit
                    if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w &&
                        y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h) {
                        restart=false;
                        quit = true;  // Khi nhấn exit, thoát khỏi trò chơi
                    }
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Màu nền đen
        SDL_RenderClear(renderer);

        // Render the game over background
        SDL_RenderCopy(renderer, gameOverBackground, NULL, NULL);

        // Render the win/loss message
        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

        // Render the restart button
        SDL_RenderCopy(renderer, restartTexture, NULL, &restartButtonRect);

        // Render the exit button
        SDL_RenderCopy(renderer, exitTexture, NULL, &exitButtonRect);

        // Update screen
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // 60 FPS
    }

    // Cleanup
    SDL_DestroyTexture(gameOverBackground);
    SDL_DestroyTexture(messageTexture);
    SDL_DestroyTexture(restartTexture);
    SDL_DestroyTexture(exitTexture);
    TTF_CloseFont(font);

    return restart;
}



#endif // GAMEOVER_H_INCLUDED
