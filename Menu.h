#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Game.h"

// Hàm hiển thị menu và trả về mức độ khó được chọn
Difficulty showMenu(SDL_Renderer* renderer, int windowWidth, int windowHeight) {
    SDL_Event e;
    bool quit = false;
    bool showDifficulty = false;  // Biến để điều khiển việc hiển thị các nút chế độ chơi
    Difficulty selectedDifficulty = EASY; // Mặc định là mức độ trung bình
    bool gameplayClicked = false;  // Biến kiểm tra trạng thái nút Gameplay đã được nhấn

    // Load font cho tên game và các nút
    TTF_Font* titleFont = TTF_OpenFont("VNI-Nhatban.ttf", 50);  // Đường dẫn đến font của bạn

    // Load background image for the menu
    SDL_Texture* backgroundTexture = loadTexture("img\\background\\backgroundmenu.png", renderer);

    // Load font for the button text
    TTF_Font* font = TTF_OpenFont("VNI-Nhatban.ttf", 30);  // Đường dẫn đến font của bạn

    // Tạo texture cho tên game
    SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "The lord of Slime ", {70, 130, 180});
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);

    // Tạo texture cho các nút độ khó
    SDL_Surface* easySurface = TTF_RenderText_Solid(font, "Easy", {255, 255, 255});
    SDL_Surface* mediumSurface = TTF_RenderText_Solid(font, "Medium", {255, 255, 255});
    SDL_Surface* hardSurface = TTF_RenderText_Solid(font, "Hard", {255, 255, 255});
    SDL_Surface* gameplaySurface = TTF_RenderText_Solid(font, "Gameplay", {255, 255, 255});

    SDL_Texture* easyTextTexture = SDL_CreateTextureFromSurface(renderer, easySurface);
    SDL_Texture* mediumTextTexture = SDL_CreateTextureFromSurface(renderer, mediumSurface);
    SDL_Texture* hardTextTexture = SDL_CreateTextureFromSurface(renderer, hardSurface);

    SDL_FreeSurface(easySurface);
    SDL_FreeSurface(mediumSurface);
    SDL_FreeSurface(hardSurface);
    SDL_FreeSurface(gameplaySurface);

    // Load texture cho nút gameplay từ ảnh
    SDL_Texture* gameplayButtonTexture = loadTexture("img\\background\\gameplay.png", renderer);  // Đảm bảo có ảnh gameplay_button.png

    // Lấy kích thước của các nút và tiêu đề
    int titleWidth, titleHeight;
    int easyTextWidth, easyTextHeight, mediumTextWidth, mediumTextHeight, hardTextWidth, hardTextHeight;
    int gameplayTextWidth, gameplayTextHeight;
    SDL_QueryTexture(titleTexture, NULL, NULL, &titleWidth, &titleHeight);
    SDL_QueryTexture(easyTextTexture, NULL, NULL, &easyTextWidth, &easyTextHeight);
    SDL_QueryTexture(mediumTextTexture, NULL, NULL, &mediumTextWidth, &mediumTextHeight);
    SDL_QueryTexture(hardTextTexture, NULL, NULL, &hardTextWidth, &hardTextHeight);
    SDL_QueryTexture(gameplayButtonTexture, NULL, NULL, &gameplayTextWidth, &gameplayTextHeight);

    // Đặt kích thước cho nút gameplay (100x100)
    int gameplayButtonWidth = 100;
    int gameplayButtonHeight = 100;

    // Đặt vị trí cho tiêu đề và các nút độ khó
    SDL_Rect titleRect = {windowWidth / 2 - titleWidth / 2, windowHeight / 4 - titleHeight / 2, titleWidth, titleHeight};
    SDL_Rect gameplayButtonRect = {windowWidth / 2 - gameplayButtonWidth / 2, windowHeight / 2 - gameplayButtonHeight / 2 + 80, gameplayButtonWidth, gameplayButtonHeight};
    SDL_Rect easyButtonRect = {windowWidth / 2 - easyTextWidth / 2, windowHeight / 2 - easyTextHeight / 2 - 50, easyTextWidth, easyTextHeight};
    SDL_Rect mediumButtonRect = {windowWidth / 2 - mediumTextWidth / 2, windowHeight / 2 - mediumTextHeight / 2 + 50, mediumTextWidth, mediumTextHeight};
    SDL_Rect hardButtonRect = {windowWidth / 2 - hardTextWidth / 2, windowHeight / 2 - hardTextHeight / 2 + 150, hardTextWidth, hardTextHeight};

    bool isGameplayHovered = false;
    bool isEasyHovered = false;
    bool isMediumHovered = false;
    bool isHardHovered = false;

    // Phát nhạc menu ban đầu
    playMenuMusic();

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Handle mouse events
            if (e.type == SDL_MOUSEMOTION) {
                int x = e.motion.x;
                int y = e.motion.y;

                // Check if the mouse is hovering over any button
                isGameplayHovered = x >= gameplayButtonRect.x && x <= gameplayButtonRect.x + gameplayButtonRect.w &&
                                    y >= gameplayButtonRect.y && y <= gameplayButtonRect.y + gameplayButtonRect.h;

                if (showDifficulty) {
                    isEasyHovered = x >= easyButtonRect.x && x <= easyButtonRect.x + easyButtonRect.w &&
                                     y >= easyButtonRect.y && y <= easyButtonRect.y + easyButtonRect.h;
                    isMediumHovered = x >= mediumButtonRect.x && x <= mediumButtonRect.x + mediumButtonRect.w &&
                                      y >= mediumButtonRect.y && y <= mediumButtonRect.y + mediumButtonRect.h;
                    isHardHovered = x >= hardButtonRect.x && x <= hardButtonRect.x + hardButtonRect.w &&
                                    y >= hardButtonRect.y && y <= hardButtonRect.y + hardButtonRect.h;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int x = e.button.x;
                    int y = e.button.y;

                    // Nếu nhấn vào nút "Gameplay", hiển thị các chế độ
                    if (x >= gameplayButtonRect.x && x <= gameplayButtonRect.x + gameplayButtonRect.w &&
                        y >= gameplayButtonRect.y && y <= gameplayButtonRect.y + gameplayButtonRect.h) {
                        gameplayClicked = true;
                        // Khi nhấn vào nút Gameplay, thu nhỏ nút đó
                        gameplayButtonWidth = 50;  // Giảm kích thước nút gameplay
                        gameplayButtonHeight = 50; // Giảm kích thước nút gameplay
                        showDifficulty = true; // Hiển thị chế độ chơi
                    }

                    // Kiểm tra nếu nhấn vào một chế độ khó
                    if (showDifficulty) {
                        if (x >= easyButtonRect.x && x <= easyButtonRect.x + easyButtonRect.w &&
                            y >= easyButtonRect.y && y <= easyButtonRect.y + easyButtonRect.h) {
                            selectedDifficulty = EASY;
                            quit = true;
                        }
                        else if (x >= mediumButtonRect.x && x <= mediumButtonRect.x + mediumButtonRect.w &&
                                 y >= mediumButtonRect.y && y <= mediumButtonRect.y + mediumButtonRect.h) {
                            selectedDifficulty = MEDIUM;
                            quit = true;
                        }
                        else if (x >= hardButtonRect.x && x <= hardButtonRect.x + hardButtonRect.w &&
                                 y >= hardButtonRect.y && y <= hardButtonRect.y + hardButtonRect.h) {
                            selectedDifficulty = HARD;
                            quit = true;
                        }
                    }
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Màu nền đen
        SDL_RenderClear(renderer);

        // Render the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Render the title
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

        if (!showDifficulty) {
            // Render the gameplay button with image (hình ảnh) và hiệu ứng thu nhỏ
            SDL_RenderCopy(renderer, gameplayButtonTexture, NULL, &gameplayButtonRect);
        } else {
            // Render difficulty buttons with hover effect
            renderButton(renderer, font, "Easy", easyButtonRect.x, easyButtonRect.y, isEasyHovered);
            renderButton(renderer, font, "Medium", mediumButtonRect.x, mediumButtonRect.y, isMediumHovered);
            renderButton(renderer, font, "Hard", hardButtonRect.x, hardButtonRect.y, isHardHovered);
        }

        // Update screen
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // 60 FPS
    }

    // Cleanup
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(easyTextTexture);
    SDL_DestroyTexture(mediumTextTexture);
    SDL_DestroyTexture(hardTextTexture);
    SDL_DestroyTexture(gameplayButtonTexture);  // Dọn dẹp ảnh của nút gameplay
    SDL_DestroyTexture(titleTexture);
    TTF_CloseFont(titleFont);
    TTF_CloseFont(font);

    return selectedDifficulty;
}

#endif // MENU_H
