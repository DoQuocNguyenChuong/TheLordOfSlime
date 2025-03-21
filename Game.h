#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include <SDL_mixer.h>

SDL_Window* window=nullptr;
SDL_Renderer* renderer=nullptr;
SDL_Texture* backgroundTexture=nullptr;
int windowWidth=800;
int windowHeight=600;
TTF_Font* font;
const char* WINDOW_TITLE = "The Lord of Slime";
// Các biến toàn cục cho âm thanh
Mix_Chunk* clickSound = nullptr;
Mix_Chunk* jumpSound = nullptr;
Mix_Chunk* winSound = nullptr;
Mix_Chunk* loseSound = nullptr;
Mix_Music* backgroundMusic = nullptr;
Mix_Music* menuMusic = nullptr;
Mix_Music* touchMusic=nullptr;
Mix_Music* slimebulletMusic=nullptr;
Mix_Music* bossbulletMusic=nullptr;


enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

// Trạng thái của trò chơi (Pause hoặc chơi bình thường)
bool isPaused = false;
bool isGameRunning = true; // Để kiểm soát trạng thái game khi dừng hoặc tiếp tục
int selectedDifficulty = EASY; // Ví dụ bạn đã có một cách để chọn difficulty

// ham khoi tao sdl
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }


    // Khởi tạo SDL_mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        std::cerr << "SDL_mixer could not open audio! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Mở font
    font = TTF_OpenFont("VNI-Nhatban.ttf", 25) ;  // Thay đường dẫn đúng đến file font của bạn
    if (!font) {
        std::cerr << "Font could not be loaded! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }


    // Khởi tạo SDL_image để hỗ trợ tải ảnh
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Tải hình ảnh nền
    backgroundTexture = IMG_LoadTexture(renderer, "img\\background\\forest.png");
    if (backgroundTexture == nullptr) {
        std::cerr << "Failed to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}


void renderText(const std::string& text, int x, int y) {
    SDL_Color color = {0, 0, 0};  // Màu den
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Unable to create text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;  // Nếu không tạo được surface, thoát khỏi hàm
    }


    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // Giải phóng surface sau khi đã tạo texture
    if (!texture) {
        std::cerr << "Unable to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
        return;  // Nếu không tạo được texture, thoát khỏi hàm
    }

    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

    // dat vi tri van ban
    SDL_Rect textRect = {x - textWidth / 2, y, textWidth, textHeight};
    // Vẽ văn bản lên màn hình
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
    // Giải phóng tài nguyên texture
    SDL_DestroyTexture(texture);
}


bool loadMedia() {
    // Tải âm thanh cho các sự kiện
    clickSound = Mix_LoadWAV("sound\\sound_mouse_click.wav");
    if (!clickSound) {
        std::cerr << "Failed to load click sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    jumpSound = Mix_LoadWAV("sound\\sound_jump_sound.wav");
    if (!jumpSound) {
        std::cerr << "Failed to load jump sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    winSound = Mix_LoadWAV("sound\\sound_win_sound.mp3");
    if (!winSound) {
        std::cerr << "Failed to load win sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    loseSound = Mix_LoadWAV("sound\\sound_lose_sound.mp3");
    if (!loseSound) {
        std::cerr << "Failed to load lose sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Tải nhạc nền
    menuMusic = Mix_LoadMUS("sound\\sound_menu_audio.wav");
    if (!menuMusic) {
        std::cerr << "Failed to load menu music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    backgroundMusic = Mix_LoadMUS("sound\\sound_bkgr_audio.wav");
    if (!backgroundMusic) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    touchMusic = Mix_LoadMUS("sound\\sound_touch_sound.wav");
    slimebulletMusic = Mix_LoadMUS("sound\\slimebullet.mp3");
    bossbulletMusic = Mix_LoadMUS("sound\\bossbullet.mp3");

    return true;
}


SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(surface);
    }
    return texture;
}


void renderDifficultyOnScreen(SDL_Renderer* renderer, TTF_Font* font, Difficulty difficulty, int windowWidth, int windowHeight) {
    // Xác định tên chế độ dựa trên lựa chọn
    std::string difficultyText;
    switch (difficulty) {
        case EASY:
            difficultyText = "Easy";
            break;
        case MEDIUM:
            difficultyText = "Medium";
            break;
        case HARD:
            difficultyText = "Hard";
            break;
    }

    // Vẽ văn bản vào vị trí trên cùng của màn hình (giữa)
    renderText(difficultyText, windowWidth / 2, 20); // Vị trí này có thể điều chỉnh theo ý muốn
}

void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height) {
    SDL_Rect destRect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}


// Hàm để vẽ nút
void renderButton(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, bool isHovered) {
    // Vẽ nền cho nút (màu nền thay đổi nếu chuột đang hover)
    SDL_Rect buttonRect = { x, y, 200, 50 };
    SDL_SetRenderDrawColor(renderer, isHovered ? 0 : 139, isHovered ? 0 : 39, 19, 255);  // Màu sáng hơn khi hover
    SDL_RenderFillRect(renderer, &buttonRect);

    // Vẽ chữ lên nút
    SDL_Color textColor = { 255, 255, 255, 255 };  // Màu chữ trắng
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = { x + (200 - textWidth) / 2, y + (50 - textHeight) / 2, textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

// Phát âm thanh khi nhấp chuột
void onClick() {
    Mix_PlayChannel(-1, clickSound, 0);  // -1 để phát âm thanh qua bất kỳ kênh nào
}

// Hàm để kiểm tra xem chuột có nhấp vào nút không
bool checkButtonClick(int mouseX, int mouseY, int buttonX, int buttonY) {
    return mouseX >= buttonX && mouseX <= buttonX + 200 && mouseY >= buttonY && mouseY <= buttonY + 50;
}

void pauseGame(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight, bool& isPaused, bool& restartGame) {
    bool quitPause = false;
    SDL_Event e;

    // Load hình nền cho màn hình Pause
    SDL_Texture* pauseBackgroundTexture = loadTexture("img\\background\\backgroundpause.png", renderer);
    if (pauseBackgroundTexture == nullptr) {
        std::cerr << "Failed to load pause background image!" << std::endl;
        return;
    }

    while (!quitPause) {
        // Vẽ nền hình ảnh (Pause)
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, pauseBackgroundTexture, nullptr, nullptr);  // Vẽ ảnh nền vào toàn bộ màn hình
        // Vẽ chữ PAUSED
        renderText("PAUSED GAME", windowWidth / 2 , windowHeight / 4 - 20);

        int mouseX, mouseY;
        bool isHoveredContinue = false;
        bool isHoveredRestart = false;

        // Lấy vị trí chuột
        SDL_GetMouseState(&mouseX, &mouseY);

        // Vẽ nút "Continue"
        if (checkButtonClick(mouseX, mouseY, windowWidth / 2 - 100, windowHeight / 2 - 25)) {
            isHoveredContinue = true;
        }
        renderButton(renderer, font, "Continue", windowWidth / 2 - 100, windowHeight / 2 - 25, isHoveredContinue);

        // Vẽ nút "Restart"
        if (checkButtonClick(mouseX, mouseY, windowWidth / 2 - 100, windowHeight / 2 + 50)) {
            isHoveredRestart = true;
        }
        renderButton(renderer, font, "Restart", windowWidth / 2 - 100, windowHeight / 2 + 50, isHoveredRestart);

        // Xử lý sự kiện chuột
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quitPause = true;
                break;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                onClick();
                if (checkButtonClick(mouseX, mouseY, windowWidth / 2 - 100, windowHeight / 2 - 25)) {
                    isPaused = false;  // Tiếp tục trò chơi
                    quitPause = true;
                }
                if (checkButtonClick(mouseX, mouseY, windowWidth / 2 - 100, windowHeight / 2 + 50)) {
                    restartGame = true;  // Bắt đầu lại trò chơi
                     quitPause = true;
                }
            }
        }

        SDL_RenderPresent(renderer);
    }
    // Dọn dẹp sau khi thoát khỏi màn hình Pause
    SDL_DestroyTexture(pauseBackgroundTexture);
}


// Phát âm thanh khi nhảy
void onJump() {
    Mix_PlayChannel(-1, jumpSound, 0);
}

// Phát âm thanh khi thắng
void onWin() {
    Mix_PlayChannel(-1, winSound, 0);
}

// Phát âm thanh khi thua
void onLose() {
    Mix_PlayChannel(-1, loseSound, 0);
}

// Phát nhạc nền khi chơi game
void playBackgroundMusic() {
    Mix_PlayMusic(backgroundMusic, -1);  // -1 để lặp lại nhạc nền mãi mãi
}

// Phát nhạc nền cho menu
void playMenuMusic() {
    Mix_PlayMusic(menuMusic, -1);  // Phát nhạc nền menu
}

void playTouchMusic(){
    Mix_PlayMusic(touchMusic,0);
}

void playSlimebulletMusic(){
    Mix_PlayMusic(slimebulletMusic,-1);
}

void playBossbulletMusic(){
    Mix_PlayMusic(bossbulletMusic,-1);
}
// Hàm giải phóng âm thanh
void freeMedia() {
    if (clickSound) {
        Mix_FreeChunk(clickSound);
        clickSound = nullptr;
    }

    if (jumpSound) {
        Mix_FreeChunk(jumpSound);
        jumpSound = nullptr;
    }

    if (winSound) {
        Mix_FreeChunk(winSound);
        winSound = nullptr;
    }

    if (loseSound) {
        Mix_FreeChunk(loseSound);
        loseSound = nullptr;
    }

    if (menuMusic) {
        Mix_FreeMusic(menuMusic);
        menuMusic = nullptr;
    }

    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }

    if(touchMusic){
        Mix_FreeMusic(touchMusic);
        touchMusic=nullptr;
    }
    if(slimebulletMusic){
        Mix_FreeMusic(slimebulletMusic);
        slimebulletMusic=nullptr;
    }
    if(bossbulletMusic){
        Mix_FreeMusic(bossbulletMusic);
        bossbulletMusic=nullptr;
    }

    Mix_Quit();
}

// Hàm dọn dẹp SDL
void close() {

    if (backgroundTexture != nullptr) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}


#endif // GAME_H_INCLUDED
