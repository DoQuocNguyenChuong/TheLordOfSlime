#include <SDL.h>
#include <iostream>
#include <vector>
#include <SDL_ttf.h>
#include<cstdlib>
#include<ctime>
#include "Game.h"
#include "Slime.h"
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "Obstacle.h"
#include "Menu.h"
#include "Gameover.h"

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(0)));

    if (!init()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return -1;
    }

   // Tải các tệp âm thanh và hình ảnh
    if (!loadMedia()) {
        std::cerr << "Failed to load media!" << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
    // Hiển thị menu và chọn độ khó
    Difficulty difficulty = showMenu(renderer, windowWidth, windowHeight);


    bool gameover=false;
    bool restartGame=false;
    int backgroundX = 0; // Vị trí cuộn của nền

    const int backgroundSpeed = 3; // Tốc độ cuộn nền

    std::vector<Obstacle> obstacles;
    std::vector<Bullet> bullets;  // Danh sách các đạn
    std::vector<Enemy> enemies;   // Danh sách các kẻ địch nhỏ
    std::vector<Boss> bosses;     // Danh sách các boss
    std::vector<BossBullet> bossBullets;  // Đạn của boss

    int enemykilled = 0;  // Đếm số lượng kẻ địch bị tiêu diệt
    bool battleWithBoss = false;
   // bool nha_dan=false;

       int slimehealth=0;
       int numObstacle = 0;
       int numEnemies = 0;
       int numToKillForBoss = 0;
       int bossHealth = 0;
       int numofbossbullet=0;
       int speedofobs=0,speedofenemies=0,speedofboss=0,speedofbossbullet=0;

       switch (difficulty) {
        case EASY:
             numObstacle = 20;    // Số lượng chướng ngại vật trong chế độ dễ
            numEnemies = 20;     // Số lượng kẻ địch trong chế độ dễ
            numToKillForBoss = 5; // Cần giết 5 kẻ địch để boss xuất hiện
            bossHealth = 50;     // Máu của boss trong chế độ dễ
            numofbossbullet=1;
            speedofobs=5;
            speedofenemies=3;
            speedofbossbullet=-5;
            speedofboss=1;
            slimehealth=10;
            timeLimit=2*60*1000;

            break;
        case MEDIUM:
            numObstacle = 40;    // Số lượng chướng ngại vật trong chế độ trung bình
            numEnemies = 40;     // Số lượng kẻ địch trong chế độ trung bình
            numToKillForBoss = 10; // Cần giết 10 kẻ địch để boss xuất hiện
            bossHealth = 100;    // Máu của boss trong chế độ trung bình
            numofbossbullet=2;
            speedofobs=10;
            speedofenemies=6;
            speedofbossbullet=-10;
            speedofboss=2;
            slimehealth=20;
            timeLimit=4*60*1000;
            break;
        case HARD:
            numObstacle = 80;    // Số lượng chướng ngại vật trong chế độ khó
            numEnemies = 80;     // Số lượng kẻ địch trong chế độ khó
            numToKillForBoss = 20; // Cần giết 20 kẻ địch để boss xuất hiện
            bossHealth = 200;    // Máu của boss trong chế độ khó
            numofbossbullet=3;
            speedofobs=15;
            speedofenemies=9;
            speedofbossbullet=-15;
            speedofboss=4;
            slimehealth=50;
            timeLimit=8*60*1000;
            break;
    }


    slime slime(slimehealth);

     // Tạo chướng ngại vật
    for (int i = 0; i < numObstacle; i++) {
        obstacles.push_back(Obstacle(400 + i * 400, 500,speedofobs));
    }

    // Tạo kẻ địch
    for (int i = 0; i < numEnemies; i++) {
        enemies.push_back(Enemy(600 + i * 300, 200 + i * 30,speedofenemies));
    }




    Uint32 lastShotTime = 0;  // Biến lưu trữ thời gian bắn lần trước
    const Uint32 shootDelay = 300;  // Thời gian trễ giữa các lần bắn (300 ms)
    playBackgroundMusic();
    // Vòng lặp game chính
    while (!gameover && !quit) {


            // Chỉ bắt đầu tính thời gian khi game được bắt đầu và không tạm dừng
        if (startTime == 0 && isPaused==false){
            startTime = SDL_GetTicks();  // Cập nhật thời gian bắt đầu game khi bắt đầu
        }

        if (isPaused) {
        // Nếu game bị pause, ghi lại thời gian bắt đầu pause
        if (pauseStartTime == 0) {
            pauseStartTime = SDL_GetTicks();  // Lưu lại thời gian khi game bị pause
        }

        // Hiển thị màn hình pause
        pauseGame(renderer, font, windowWidth, windowHeight, isPaused, restartGame);

        if (restartGame) {
            // Nếu người chơi chọn restart, reset thời gian
            startTime = 0;
            totalPauseTime = 0;
            slime.health = 0;  // Reset slime health
        }
    } else {
        // Nếu game không pause, tính toán thời gian đã trôi qua
        if (pauseStartTime > 0) {
            totalPauseTime += SDL_GetTicks() - pauseStartTime;  // Cộng thêm thời gian pause vào tổng thời gian pause
            pauseStartTime = 0;  // Reset thời gian pause
        }
    }


        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                slime.health=0;
                quit = true;
            }

             bool di_trai=false;
             bool di_phai=false;
            if (e.type == SDL_KEYDOWN) {


                if (e.key.keysym.sym == SDLK_p) {
                        isPaused = !isPaused;  // Đảo ngược trạng thái tạm dừng khi nhấn phím P
                    }

                if (!isPaused) {
                if (e.key.keysym.sym == SDLK_UP){
                    slime.jump();  // Nhảy khi nhấn up
                }

                if (e.key.keysym.sym == SDLK_LEFT) {
                    if(!di_trai){
                     slime.left();  // trai khi nhấn left
                     di_trai=true;
                    }
                }

                if (e.key.keysym.sym == SDLK_RIGHT) {
                    if(!di_phai){
                     slime.right();  // trai khi nhấn right
                     di_phai=true;
                    }
                }
                // Kiểm tra điều kiện bắn đạn (chỉ bắn nếu thời gian trễ đã đủ)
                if (e.key.keysym.sym == SDLK_w && SDL_GetTicks() - lastShotTime > shootDelay) {
                  playSlimebulletMusic();
                  int x1 = slime.x + slime.w;
                  int y1 = slime.y + slime.h / 2 - 2;
                  bullets.push_back(Bullet(x1, y1));  // Tạo đạn tại vị trí slime
                  lastShotTime = SDL_GetTicks();  // Cập nhật thời gian bắn
                   }
                  }
               }

            if (e.type == SDL_KEYUP) {
                 if (e.key.keysym.sym == SDLK_LEFT) {
                     di_trai = false;
                     slime.stopLeft();  // Dừng di chuyển sang trái khi nhả phím
                  }
             if (e.key.keysym.sym == SDLK_RIGHT) {
                    di_phai = false;
                    slime.stopRight();  // Dừng di chuyển sang phải khi nhả phím
                  }
              }
        }


        backgroundX -= backgroundSpeed;

        // Nếu nền đã ra khỏi màn hình, đưa nó về vị trí ban đầu
        if (backgroundX <= -windowWidth) {
            backgroundX = 0;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Màu đen (hoặc bất kỳ màu nào bạn muốn cho nền)
        SDL_RenderClear(renderer);  // Xóa màn hình


        // Cập nhật vị trí cuộn nền
        backgroundX -= backgroundSpeed;
        if (backgroundX <= -windowWidth) {
           backgroundX = 0;
        }

        SDL_Rect backgroundRect1 = {backgroundX, 0, windowWidth, windowHeight};  // Vị trí nền đầu tiên
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect1);

        // Vẽ nền tiếp theo sau nền đầu tiên
        SDL_Rect backgroundRect2 = {backgroundX + windowWidth, 0, windowWidth, windowHeight};  // Vị trí nền tiếp theo
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect2);

         if (isPaused==false) {
        // Tính toán elapsedTime, trừ đi tổng thời gian pause
    elapsedTime = SDL_GetTicks() - startTime - totalPauseTime;  // Trừ đi thời gian pause

         // Tính thời gian còn lại
        uint32_t remainingTime = timeLimit - elapsedTime;
        uint32_t secondsRemaining = remainingTime / 1000;  // Thời gian còn lại (giây)

         // Hiển thị thời gian đếm ngược
         std::string timeText = "Time limited: " + std::to_string(secondsRemaining) + "  s";  // Thời gian còn lại
         renderText(timeText, 200, 0);  // Hiển thị thời gian ở góc trên bên trái
}
            // Kiểm tra nếu hết thời gian giới hạn
            if (elapsedTime >= timeLimit) {
                slime.health=0;
                onLose();
                gameover = true;  // Kết thúc game nếu hết thời gian
            }







        // Di chuyển slime
        slime.move();

        // Di chuyển chướng ngại vật va xoa cac chuong ngai vat khi ra khoi man hinh
        for (auto it = obstacles.begin(); it != obstacles.end(); ) {
          it->move();
          if (it->outofscreen()) {
               it = obstacles.erase(it);  // Erase and move to the next bullet
               if(it==obstacles.end()){
                break;
               }
          } else {
               ++it;  // Move to the next bullet if not erased
            }
          }

        // Di chuyển các đạn va xoa cac dan khi di chuyen ra khoi man hinh
        for (auto it = bullets.begin(); it != bullets.end(); ) {
          it->move();
          if (it->outofscreen()) {
               it = bullets.erase(it);  // Erase and move to the next bullet
               if(it==bullets.end()){
                break;
               }
          } else {
               ++it;  // Move to the next bullet if not erased
            }
          }
        // Di chuyển các kẻ địch nhỏ
        if (!battleWithBoss) {
            for (auto& enemy : enemies) {
                enemy.move(slime);
            }
        }

        // Di chuyển boss nếu có
        for (auto& boss : bosses) {
            boss.move(slime);
        }

        // Boss bắn đạn
                // Boss bắn đạn
        if (battleWithBoss) {
            if (rand() % 100 < numofbossbullet) {  // Tỉ lệ xuất hiện đạn của boss
                bossBullets.push_back(BossBullet(bosses[0].x, bosses[0].y + 30,speedofbossbullet));
                // Khi boss bắn đạn, bật chế độ hoạt hình (start shooting)
                bosses[0].startShooting();
                 playBossbulletMusic();
            }
            for (auto it = bossBullets.begin(); it != bossBullets.end(); ) {
               it->move();  // Move the bullet

                if (it->outofscreen()) {
                     it = bossBullets.erase(it);  // xoa dan va khoi tao dan moi
                     if(it == bossBullets.end()){
                         break;
                     }
                } else {
                     ++it;
                }
            }

            // Kiểm tra trạng thái bắn đạn và cập nhật sprite sheet
            if (bossBullets.empty()) {
                // Nếu không còn đạn, dừng việc bắn và dừng hoạt hình
                bosses[0].stopShooting();
            }
        }


        // Kiểm tra va chạm giữa đạn và kẻ địch nhỏ
        for (auto it = bullets.begin(); it != bullets.end();) {
          bool bulletDestroyed = false;
          for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
        if (it->checkCollisionWith(*enemyIt) ){
            enemyIt = enemies.erase(enemyIt);  // Xóa kẻ địch khi bị trúng đạn
            if(it==bullets.end()){
               break;
            }
            bulletDestroyed = true;
            enemykilled++;  // Tăng số lượng kẻ địch bị tiêu diệt
            break;
        }
        else {
            ++enemyIt;
        }
    }
    if (bulletDestroyed) {
        it = bullets.erase(it);  // Xóa đạn sau khi va chạm
        if(it==bullets.end()){
            break;
            }
    } else {
        ++it;
    }
}

       //kiem tra va cham giua dan va obstacle
        for (auto it = bullets.begin(); it != bullets.end();) {
    bool bulletDestroyed = false;
    for (auto obstacleIt = obstacles.begin(); obstacleIt != obstacles.end();) {
        if (it->checkCollisionWith(*obstacleIt) ){
            bulletDestroyed = true;
            break;
        }
        else {
            ++obstacleIt;
        }
    }
    if (bulletDestroyed) {
        it = bullets.erase(it);// Xóa đạn sau khi va chạm
        if(it==bullets.end()){
            break;
        }
    } else {
        ++it;
    }
}

        // Kiểm tra va chạm giữa đạn và boss
        for (auto it = bullets.begin(); it != bullets.end();) {
            bool bulletDestroyed = false;
            for (auto& boss : bosses) {
                if (it->checkCollisionWith(boss)) {
                    boss.health -= 1;  // Boss mất 1 máu mỗi lần bị trúng đạn
                    bulletDestroyed = true;
                    if (boss.health <= 0) {
                        quit = true;  // Kết thúc game khi boss chết
                    }
                    break;
                }
            }


            if (bulletDestroyed) {
                it = bullets.erase(it);  // Xóa đạn sau khi va chạm
                if(it==bullets.end()){
                break;
            }
            } else {
                ++it;
            }
        }

        // Kiểm tra va chạm giữa đạn của boss và slime
        for (auto it = bossBullets.begin(); it != bossBullets.end();) {
            if (it->checkCollisionWith(slime)) {
                playTouchMusic();
                slime.health -= 1;  // Dino mất 1 máu khi trúng đạn của boss
                it = bossBullets.erase(it);// Xóa đạn của boss sau khi va chạm
                if(it==bossBullets.end()){
                    break;
                }
            } else {
                ++it;
            }
        }

        // Kiểm tra va chạm giữa chướng ngại vật và slime
        for (auto it = obstacles.begin(); it != obstacles.end();) {
            if (it->checkCollisionWith(slime)) {
                playTouchMusic();
                slime.health -= 1;  // Dino mất 1 máu khi chạm vào chướng ngại vật
                it = obstacles.erase(it);  // Xóa chướng ngại vật khi chạm vào Dino
                if(it==obstacles.end()){
                    break;
                }
            }
            else {
                ++it;
            }
       }

        // Kiểm tra va chạm giữa kẻ địch và slime
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (it->checkCollisionWith(slime)) {
                playTouchMusic();
                slime.health -= 1;  // Dino mất 1 máu khi chạm vào kẻ địch
                it = enemies.erase(it);  // Xóa kẻ địch khi chạm vào slime
                if(it==enemies.end()){
                    break;
                }
            } else {
                ++it;
            }
        }

        // Duyệt qua tất cả các viên đạn của slime
for (auto it = bullets.begin(); it != bullets.end();) {
    bool collisionDetected = false;

    // Kiểm tra va chạm với tất cả các viên đạn của boss
    for (auto bossIt = bossBullets.begin(); bossIt != bossBullets.end(); ) {
        if (it->checkCollisionWith(*bossIt)) {  // Kiểm tra va chạm
            // Nếu va chạm xảy ra, xóa cả viên đạn của slime và boss
            it = bullets.erase(it);   // Xóa viên đạn của slime
            bossIt = bossBullets.erase(bossIt);  // Xóa viên đạn của boss
            collisionDetected = true;
            break;  // Không cần kiểm tra va chạm với các viên đạn boss khác nữa
        } else {
            ++bossIt;  // Nếu không va chạm, tiếp tục kiểm tra với viên đạn boss kế tiếp
        }
    }

    // Nếu không có va chạm, tiếp tục duyệt viên đạn của slime
    if (!collisionDetected) {
        ++it;
    }
}

        // Kiểm tra xem số lượng kẻ địch nhỏ đã đạt 10 chưa
        if (enemykilled >= numToKillForBoss  && bosses.empty()) {
            bosses.push_back(Boss(bossHealth,speedofboss));  // Tạo boss khi đủ 10 kẻ địch nhỏ bị tiêu diệt
            enemies.clear();  // Xóa hết kẻ địch nhỏ
            obstacles.clear(); // xoa het chuong ngai vat;
            battleWithBoss = true;  // Dừng màn hình cho trận chiến với boss
        }

        // Kiểm tra xem slime có hết máu không
        if (slime.health <= 0) {
            onLose();
            gameover = true;  // Exit the game
        }

         // Nếu boss bị đánh bại
       for (auto& boss : bosses) {
             if (boss.health <= 0) {
                onWin();
                gameover = true;  // End the game loop
           }
        }

//        // Vẽ lại màn hình
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Màu nền trắng
//        SDL_RenderClear(renderer);

        // Vẽ slime, đạn, chướng ngại vật, kẻ địch nhỏ và boss
        slime.draw();
        slime.drawHealthBar();  // Vẽ thanh máu
        for (auto& obstacle : obstacles) {
            obstacle.draw();
        }
        for (auto& bullet : bullets) {
            bullet.draw();
        }
        for (auto& enemy : enemies) {
            enemy.draw();
        }
        for (auto& boss : bosses) {
            boss.draw();
            boss.drawHealthBar();
        }
        for (auto& bossBullet : bossBullets) {
            bossBullet.draw();
        }


        if (isPaused) {
                playBackgroundMusic();
                // Hiển thị màn hình tạm dừng
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // Vẽ chữ "PAUSE" lên màn hình
                renderText("PAUSED", windowWidth / 2 - 50, windowHeight / 2 - 20);

                SDL_RenderPresent(renderer);
                SDL_Delay(100);  // Để không tốn tài nguyên quá mức
                continue;  // Dừng vòng lặp chính để không tiếp tục cập nhật khi tạm dừng
            }




        // Render số lượng kẻ địch bị giết
        renderText("Enemies Killed: " + std::to_string(enemykilled), 400, 100);
//        // Hiển thị chế độ hiện tại (EASY, MEDIUM, HARD)
        renderDifficultyOnScreen(renderer, font, difficulty, windowWidth, windowHeight);

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);


        // Giới hạn tốc độ khung hình
        SDL_Delay(16);  // Khoảng 60 FPS

    }

        // When the game ends, show the game over screen
       restartGame = showGameOverScreen(renderer, windowWidth, windowHeight, slime.health > 0);

        if (restartGame ) {
            startTime = 0;
            quit=false;
        } else {
            quit = true;  // Exit the game and go back to the menu
        }
    }
    freeMedia();
    close();
    return 0;
}
