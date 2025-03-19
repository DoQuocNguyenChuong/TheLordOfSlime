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


int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(0)));

    if (!init()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    int backgroundX = 0; // Vị trí cuộn của nền
    const int backgroundSpeed = 3; // Tốc độ cuộn nền

    std::vector<Obstacle> obstacles;
    std::vector<Bullet> bullets;  // Danh sách các đạn
    std::vector<Enemy> enemies;   // Danh sách các kẻ địch nhỏ
    std::vector<Boss> bosses;     // Danh sách các boss
    std::vector<BossBullet> bossBullets;  // Đạn của boss

    slime slime;


    int enemykilled = 0;  // Đếm số lượng kẻ địch bị tiêu diệt
    bool battleWithBoss = false;
   // bool nha_dan=false;
    bool di_trai=false;
    bool di_phai=false;

    int numObstacle = rand()%20+30;
    for(int i=0;i<numObstacle;i++){
        // Thêm một enemy mới vào danh sách enemies
        obstacles.push_back(Obstacle(400+i*400,500));
    }

    int numEnemies = rand() % 20+ 30;
    for(int i=0;i<numEnemies;i++){
        // Thêm một enemy mới vào danh sách enemies
        enemies.push_back(Enemy(600+i*300,200+i*30));
    }

    Uint32 lastShotTime = 0;  // Biến lưu trữ thời gian bắn lần trước
    const Uint32 shootDelay = 300;  // Thời gian trễ giữa các lần bắn (300 ms)

    // Vòng lặp game chính
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
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
                  int x1 = slime.x + slime.w;
                  int y1 = slime.y + slime.h / 2 - 2;
                  bullets.push_back(Bullet(x1, y1));  // Tạo đạn tại vị trí slime
                  lastShotTime = SDL_GetTicks();  // Cập nhật thời gian bắn
                   }


//                if (e.key.keysym.sym == SDLK_w){
//                    if(!nha_dan){
//                    int x1=slime.x + slime.w;
//                    int y1= slime.y + slime.h / 2 - 2;
//                    bullets.push_back(Bullet(x1, y1));  // Vị trí bắn từ giữa nhân vật
//                    nha_dan=true;
//                    }
//                 }
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

//             if (e.key.keysym.sym == SDLK_w) {
//                  nha_dan = false;
//                  }
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


        // Di chuyển Dino
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
            if (rand() % 100 < 1) {  // Tỉ lệ xuất hiện đạn của boss
                bossBullets.push_back(BossBullet(bosses[0].x, bosses[0].y + 30));
                // Khi boss bắn đạn, bật chế độ hoạt hình (start shooting)
                bosses[0].startShooting();
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
                        renderText("You Win! Boss defeated!", 300, 200);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(2000);  // Hiển thị Game Over trong 2 giây
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
        if (enemykilled >= 10 && bosses.empty()) {
            bosses.push_back(Boss());  // Tạo boss khi đủ 10 kẻ địch nhỏ bị tiêu diệt
            enemies.clear();  // Xóa hết kẻ địch nhỏ
            obstacles.clear(); // xoa het chuong ngai vat;
            battleWithBoss = true;  // Dừng màn hình cho trận chiến với boss
        }

        // Dừng game khi slime hết máu
            if (slime.health <= 0) {
               renderText("Game Over! You died!", 300, 200);
               std::cout << "Game Over! slime died!" << std::endl;
               SDL_RenderPresent(renderer);
               SDL_Delay(2000);  // Hiển thị Game Over trong 2 giây
               quit = true;
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

        // Render số lượng kẻ địch bị giết
        renderText("Enemies Killed: " + std::to_string(enemykilled), 400, 25);


        // Cập nhật màn hình
        SDL_RenderPresent(renderer);

        // Giới hạn tốc độ khung hình
        SDL_Delay(16);  // Khoảng 60 FPS
    }

    close();
    return 0;
}
