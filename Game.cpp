//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "Game.h"

#include <iostream>
#include <ostream>
#include <random>
#include <bits/this_thread_sleep.h>

#include "ingameobj/Ball.h"
#include "ingameobj/Block.h"

Game::Game(const int width, const int height): width(width), height(height), startX(width / 2) {
    main_arrow_helper = new ArrowHelper(50, 350, 100, 0, 91, 168, 244, 128);
    main_arrow_helper->setSkipRender(true);
    objects.push_back(main_arrow_helper);
}

Game::~Game() {
    // release ImageRenderable objects
    for (ImageRenderable* &it : objects) {
        delete it;
    }
}


void Game::gameStart(const int level) {
    // init level
    this->level = level;
    int empty_position = rand() % max_block_cnt_x;

    int block_width = (width - block_margin * max_block_cnt_x * 2) / max_block_cnt_x;
    int block_height = (ingameHeight - block_margin * max_block_cnt_y * 2) / max_block_cnt_y;

    std::cout << "block_width: " << block_width << ", block_height: " << block_height << std::endl;

    {
        std::lock_guard lg2(blocksMutex);
        //std::cout << "phase 0" << std::endl;
        for (int i = 0; i < max_block_cnt_x; i++) {
            if (i == empty_position) {
                continue;
            }
            //std::cout << "phase 12" << std::endl;

            auto* block = new Block(block_margin + i * (block_width + block_margin), block_margin, block_width, block_height, level, level, 255, 0, 0);

            //std::cout << "phase 13" << std::endl;
            objects.push_back(block);
            //std::cout << "phase 14" << std::endl;
            blocks.push_back(block);
            //std::cout << "phase 15" << std::endl;

            // std::cout << block->getSkipRender() << std::endl; // ( false
        }
    }

    // std::cout << "phase 1" << std::endl;
    std::cout << "blocks cnt: " << blocks.size() << std::endl;

    auto* ball = new Ball(startX, height - 6, 6, 91, 168, 244, 255);
    if (main_ball == nullptr) {
        main_ball = ball;
    }
    else {
        auto& mainBallPoint = main_ball->getPoint();
        auto& newBallPoint = ball->getPoint();
        newBallPoint.set_xy(mainBallPoint.get_x(), mainBallPoint.get_y());
    }

    {
        std::lock_guard lg(ballsMutex);
        balls.push_back(ball);
    }
    objects.push_back(ball);
}

void Game::gameOver() {
    std::lock_guard lg2(blocksMutex);
    for(Block *block : blocks) {
        block->setRGBA(30, 30, 30, 255);
    }
}

bool Game::isGameOver() {
    std::lock_guard lg2(blocksMutex);
    int i = 0;
    for(Block *block : blocks) {
        if (i >= 6) {
            break;
        }

        auto& point = block->getPoint();
        auto end = block->getHeight() + point.get_y();
        if (height <= end) {
            return true;
        }

        std::cout << "height: " << height << std::endl;
        std::cout << "end: " << end << std::endl;

        i++;
    }

    return false;
}

std::list<ImageRenderable*>* Game::getObjectsPtr() {
    return &this->objects;
}

void Game::collisionWall() {
    std::lock_guard lg(ballsMutex);
    for(Ball *&ball : balls) {
        R2Point &ballPoint = ball->getPoint();

        double r = ball->getRadius();
        double bx = ballPoint.get_x();
        double by = ballPoint.get_y();

        if (bx <= r) {
            ballPoint.set_x(r);
            ball->reverseDx();
        }
        if (width - bx <= r) {
            ballPoint.set_x(width - r);
            ball->reverseDx();
        }
        if (by <= r) {
            ballPoint.set_y(r);
            ball->reverseDy();
        }
        if (height - by <= r) {
            startX = bx;
            ball->setdx(0);
            ball->setdy(0);
            ballPoint.set_y(height - r);
        }
    }
}
void Game::collisionBlock() {
    std::lock_guard lg(ballsMutex);
    for(Ball *ball : balls) {
        std::lock_guard lg2(blocksMutex);
        for(Block *block : blocks) {
            int num = block->meet(*ball);
            // std::cout << block->getPoint().get_x() << ", " << block->getPoint().get_y() << " //// ";

            if (num != 0) {
                // block->hit(*ball);
                int hp = block->getHp() - 1;
                block->setHp(hp);

                if(num == 1) {
                    ball->reverseDx();
                }
                else if(num == 2) {
                    ball->reverseDy();
                }
                else if(num == 3) {
                    ball->reverseDx();
                    ball->reverseDy();
                }
                // ball->move();

                if (hp <= 0) {
                    blocks.remove(block);
                    objects.remove(block);
                    delete block;
                }

                break;
            }
        }

        // std::cout << std::endl;
    }
}

int Game::nextLevel() {
    level += 1;
    // std::cout << "next level called" << std::endl;

    {
        std::lock_guard lg2(blocksMutex);

        for (Block *block : blocks) {
            R2Point &point = block->getPoint();
            point.set_y(point.get_y() + block->getHeight() + block_margin);
        }

        int empty_position = rand() % max_block_cnt_x;
        int block_width = (width - block_margin * max_block_cnt_x * 2) / max_block_cnt_x;
        int block_height = (ingameHeight - block_margin * max_block_cnt_y * 2) / max_block_cnt_y;
        for (int i = 0; i < max_block_cnt_x; i++) {
            if (i == empty_position) {
                continue;
            }
            //std::cout << "phase 12" << std::endl;

            auto* block = new Block(block_margin + i * (block_width + block_margin), block_margin, block_width, block_height, level, level, 255, 0, 0);

            //std::cout << "phase 13" << std::endl;
            objects.push_back(block);
            //std::cout << "phase 14" << std::endl;
            blocks.push_back(block);
            //std::cout << "phase 15" << std::endl;

            // std::cout << block->getSkipRender() << std::endl; // ( false
        }
    }

    // gameStart(level + 1);
    auto* ball = new Ball(startX, 350, 6, 91, 168, 244, 255);
    auto& mainBallPoint = main_ball->getPoint();
    auto& newBallPoint = ball->getPoint();
    newBallPoint.set_xy(mainBallPoint.get_x(), mainBallPoint.get_y());

    balls.push_back(ball);
    objects.push_back(ball);

    // collect ball at one point
    for (Ball *&ball : balls) {
        auto& point = ball->getPoint();
        // for (int i = point.get_x(); i < startX; i++) {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //     point.set_x(i);
        // }
        point.set_x(startX);
    }

    // check game over
    if (isGameOver()) {
        gameOver();
    }

    // todo:// check if game over or not
    return 1;
}

void Game::render(png_bytep* png_bytep_data) {
    // std::cout << main_arrow_helper->getSkipRender() << std::endl;
    for (ImageRenderable* &it : objects) {
        if (it->getSkipRender()) {
            continue;
        }

        it->render(width, height, png_bytep_data);
        // std::cout << "rendering: "<< it->getSkipRender() << std::endl;
    }

    collisionWall();
    collisionBlock();
}

void Game::fire() {
    double norm = sqrt(dx * dx + dy * dy) / 10;
    if (norm == 0) {
        return;
    }

    phaseRunning.store(true);

    std::list<Ball*> localBalls;
    {
        std::lock_guard lg(ballsMutex);
        localBalls = balls;   // 포인터 목록만 복사
    }

    for (Ball* &ball : localBalls) {
        ball->setdx(dx / norm);
        ball->setdy(dy / norm);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Game::checkCurrentPhaseOver() {
    if (phaseRunning.load()) {
        std::lock_guard lg(ballsMutex);
        // ball radius;
        bool tf = true;
        for (Ball* &ball : balls) {
            if (ball->getdx() != 0 || ball->getdy() != 0) {
                tf = false;
            }
        }

        // std::cout << "Game Over: " << tf << std::endl;
        if (tf) {
            phaseRunning.store(false);
            nextLevel();
        }
    }

    // std::cout << "phaseRunning: " << phaseRunning.load() << std::endl;
}


void Game::onMouseLeftDown(const wxMouseEvent &e) {
    const auto p = e.GetPosition();
    this->mouse_down_x = p.x;
    this->mouse_down_y = p.y;

    main_arrow_helper->setSkipRender(false);
    R2Point &point = main_arrow_helper->getPoint();
    R2Point &ballPoint = main_ball->getPoint();
    point.set_xy(ballPoint.get_x(), ballPoint.get_y());
    //std::cout << "game: mouse down" << std::endl;
}

void Game::onMouseLeftUp(const wxMouseEvent &e) {
    main_arrow_helper->setSkipRender(true);
    fire();
    //std::cout << "game: mouse up" << std::endl;
}

void Game::onMouseMove(const wxMouseEvent &e) {
    //std::cout << "mouse move" << std::endl;
    //std::cout << e.Dragging() << ", , " << e.LeftIsDown() << std::endl;
    if (e.Dragging() && e.LeftIsDown()) {
        const wxPoint p = e.GetPosition();

        dx = p.x - mouse_down_x;
        dy = p.y - mouse_down_y;
        if (dx == 0) {
            return;
        }

        // double slope = static_cast<double>(dy) / dx;
        const double theta = -atan2(dy, dx);

        // std::cout << p.x << ", " << p.y << ", 기울기: " << slope << std::endl;

        main_arrow_helper->setTheta(theta);
    }
}


