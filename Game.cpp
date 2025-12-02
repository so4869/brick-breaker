//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "Game.h"

#include <iostream>
#include <ostream>
#include <random>
#include <thread>

#include "ingameobj/Ball.h"
#include "ingameobj/Block.h"

Game::Game(const int width, const int height): width_(width), height_(height), start_x_(width / 2) {
    main_arrow_helper_ = new ArrowHelper(50, 350, 100, 0, 91, 168, 244, 128);
    main_arrow_helper_->setSkipRender(true);
    objects_.push_back(main_arrow_helper_);
}

Game::~Game() {
    // release ImageRenderable objects
    for (ImageRenderable* &it : objects_) {
        delete it;
    }
}


void Game::init() {
    {
        std::lock_guard lg(balls_mutex_);
        balls_.clear();
    }
    {
        std::lock_guard lg(blocks_mutex_);
        blocks_.clear();
    }

    for (ImageRenderable* &it : objects_) {
        delete it;
    }
    objects_.clear();

    main_ball_ = nullptr;
}


void Game::gameStart(const int level) {

    // init level
    this->level_ = level;
    int empty_position = rand() % max_block_cnt_x_;

    int block_width = (width_ - block_margin_ * max_block_cnt_x_ * 2) / max_block_cnt_x_;
    int block_height = (ingame_height_ - block_margin_ * max_block_cnt_y_ * 2) / max_block_cnt_y_;

    // std::cout << "block_width: " << block_width << ", block_height: " << block_height << std::endl;

    {
        std::lock_guard lg2(blocks_mutex_);
        //std::cout << "phase 0" << std::endl;
        for (int i = 0; i < max_block_cnt_x_; i++) {
            if (i == empty_position) {
                continue;
            }
            //std::cout << "phase 12" << std::endl;

            auto* block = new Block(block_margin_ + i * (block_width + block_margin_), block_margin_, block_width, block_height, level, level, 255, 0, 0);

            //std::cout << "phase 13" << std::endl;
            objects_.push_back(block);
            //std::cout << "phase 14" << std::endl;
            blocks_.push_back(block);
            //std::cout << "phase 15" << std::endl;

            // std::cout << block->getSkipRender() << std::endl; // ( false
        }
    }

    // std::cout << "phase 1" << std::endl;
    // std::cout << "blocks cnt: " << blocks_.size() << std::endl;

    auto* ball = new Ball(start_x_, height_ - 6, 6, 91, 168, 244, 255);
    if (main_ball_ == nullptr) {
        main_ball_ = ball;
    }
    else {
        auto& mainBallPoint = main_ball_->getPoint();
        auto& newBallPoint = ball->getPoint();
        newBallPoint.setXY(mainBallPoint.get_x(), mainBallPoint.get_y());
    }

    {
        std::lock_guard lg(balls_mutex_);
        balls_.push_back(ball);
    }
    objects_.push_back(ball);

    game_over_.store(false);
}

void Game::gameOver() {
    std::lock_guard lg2(blocks_mutex_);
    for(Block *block : blocks_) {
        block->setRGBA(30, 30, 30, 255);
    }

    game_over_.store(true);
}

bool Game::isGameOver() {
    return game_over_.load();
}

bool Game::checkGameOver() {
    std::lock_guard lg2(blocks_mutex_);
    int i = 0;
    for(Block *block : blocks_) {
        if (i >= 6) {
            break;
        }

        auto& point = block->getPoint();
        auto end = block->getHeight() + point.get_y();
        if (height_ <= end) {
            return true;
        }

        // std::cout << "height: " << height << std::endl;
        // std::cout << "end: " << end << std::endl;

        i++;
    }

    return false;
}

std::list<ImageRenderable*>* Game::getObjectsPtr() {
    return &this->objects_;
}

void Game::collisionWall() {
    std::lock_guard lg(balls_mutex_);
    for(Ball *&ball : balls_) {
        R2Point &ballPoint = ball->getPoint();

        double r = ball->getRadius();
        double bx = ballPoint.get_x();
        double by = ballPoint.get_y();

        if (bx <= r) {
            ballPoint.setX(r);
            ball->reverseDx();
        }
        if (width_ - bx <= r) {
            ballPoint.setX(width_ - r);
            ball->reverseDx();
        }
        if (by <= r) {
            ballPoint.setY(r);
            ball->reverseDy();
        }
        if (height_ - by <= r) {
            start_x_ = bx;
            ball->setDx(0);
            ball->setDy(0);
            ballPoint.setY(height_ - r);
        }
    }
}
void Game::collisionBlock() {
    std::lock_guard lg(balls_mutex_);
    for(Ball *ball : balls_) {
        std::lock_guard lg2(blocks_mutex_);
        for(Block *block : blocks_) {
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
                    blocks_.remove(block);
                    objects_.remove(block);
                    delete block;
                }

                break;
            }
        }

        // std::cout << std::endl;
    }
}

int Game::nextLevel() {
    level_ += 1;
    // std::cout << "next level called" << std::endl;

    {
        std::lock_guard lg2(blocks_mutex_);

        for (Block *block : blocks_) {
            R2Point &point = block->getPoint();
            point.setY(point.get_y() + block->getHeight() + block_margin_);
        }

        int empty_position = rand() % max_block_cnt_x_;
        int block_width = (width_ - block_margin_ * max_block_cnt_x_ * 2) / max_block_cnt_x_;
        int block_height = (ingame_height_ - block_margin_ * max_block_cnt_y_ * 2) / max_block_cnt_y_;
        for (int i = 0; i < max_block_cnt_x_; i++) {
            if (i == empty_position) {
                continue;
            }
            //std::cout << "phase 12" << std::endl;

            auto* block = new Block(block_margin_ + i * (block_width + block_margin_), block_margin_, block_width, block_height, level_, level_, 255, 0, 0);

            //std::cout << "phase 13" << std::endl;
            objects_.push_back(block);
            //std::cout << "phase 14" << std::endl;
            blocks_.push_back(block);
            //std::cout << "phase 15" << std::endl;

            // std::cout << block->getSkipRender() << std::endl; // ( false
        }
    }

    // gameStart(level + 1);
    auto* ball = new Ball(start_x_, 350, 6, 91, 168, 244, 255);
    auto& mainBallPoint = main_ball_->getPoint();
    auto& newBallPoint = ball->getPoint();
    newBallPoint.setXY(mainBallPoint.get_x(), mainBallPoint.get_y());

    balls_.push_back(ball);
    objects_.push_back(ball);

    // collect ball at one point
    for (Ball *&ball : balls_) {
        auto& point = ball->getPoint();
        // for (int i = point.get_x(); i < startX; i++) {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //     point.set_x(i);
        // }
        point.setX(start_x_);
    }

    // check game over
    if (checkGameOver()) {
        gameOver();
    }

    // todo:// check if game over or not
    return 1;
}

void Game::render(png_bytep* png_bytep_data) {
    // std::cout << main_arrow_helper->getSkipRender() << std::endl;
    for (ImageRenderable* &it : objects_) {
        if (it->getSkipRender()) {
            continue;
        }

        it->render(width_, height_, png_bytep_data);
        // std::cout << "rendering: "<< it->getSkipRender() << std::endl;
    }

    collisionWall();
    collisionBlock();
}

void Game::fire() {
    double norm = sqrt(dx_ * dx_ + dy_ * dy_) / 10;
    if (norm == 0) {
        return;
    }

    phase_running_.store(true);

    std::list<Ball*> localBalls;
    {
        std::lock_guard lg(balls_mutex_);
        localBalls = balls_;   // 포인터 목록만 복사
    }

    for (Ball* &ball : localBalls) {
        ball->setDx(dx_ / norm);
        ball->setDy(dy_ / norm);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Game::checkCurrentPhaseOver() {
    if (phase_running_.load()) {
        std::lock_guard lg(balls_mutex_);
        // ball radius;
        bool tf = true;
        for (Ball* &ball : balls_) {
            if (ball->getDx() != 0 || ball->getDy() != 0) {
                tf = false;
            }
        }

        // std::cout << "Game Over: " << tf << std::endl;
        if (tf) {
            phase_running_.store(false);
            nextLevel();
        }
    }

    // std::cout << "phaseRunning: " << phaseRunning.load() << std::endl;
}


void Game::onMouseLeftDown(const wxMouseEvent &e) {
    if (isGameOver()) {
        return;
    }

    const auto p = e.GetPosition();
    this->mouse_down_x_ = p.x;
    this->mouse_down_y_ = p.y;

    main_arrow_helper_->setSkipRender(false);
    R2Point &point = main_arrow_helper_->getPoint();
    R2Point &ballPoint = main_ball_->getPoint();
    point.setXY(ballPoint.get_x(), ballPoint.get_y());
    //std::cout << "game: mouse down" << std::endl;
}

void Game::onMouseLeftUp(const wxMouseEvent &e) {
    if (isGameOver()) {
        init();
        gameStart(1);

        return;
    }

    main_arrow_helper_->setSkipRender(true);

    std::thread t([this]() {
        fire();
    });
    t.detach();
    //std::cout << "game: mouse up" << std::endl;
}

void Game::onMouseMove(const wxMouseEvent &e) {
    if (isGameOver()) {
        return;
    }

    //std::cout << "mouse move" << std::endl;
    //std::cout << e.Dragging() << ", , " << e.LeftIsDown() << std::endl;
    if (e.Dragging() && e.LeftIsDown()) {
        const wxPoint p = e.GetPosition();

        dx_ = p.x - mouse_down_x_;
        dy_ = p.y - mouse_down_y_;
        if (dx_ == 0) {
            return;
        }

        // double slope = static_cast<double>(dy) / dx;
        const double theta = -atan2(dy_, dx_);

        // std::cout << p.x << ", " << p.y << ", 기울기: " << slope << std::endl;

        main_arrow_helper_->setTheta(theta);
    }
}


