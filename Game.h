//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef GAME_H
#define GAME_H
#include <atomic>
#include <list>
#include <mutex>
#include <wx/event.h>

#include "ingameobj/ArrowHelper.h"
#include "ingameobj/Ball.h"
#include "ingameobj/Block.h"
#include "ingameobj/ImageRenderable.h"


class Game {
private:
    const int width_, height_;
    int start_x_;
    const int ingame_height_ = height_ - 140;
    int max_block_cnt_x_ = 6;
    int max_block_cnt_y_ = 8;

    int block_margin_ = 3;

    // todo://뭔지 모르겟으나 이따 화긴
    int start_stage_ = 0;

    std::list<ImageRenderable*> objects_;

    std::mutex balls_mutex_;
    std::list<Ball*> balls_;
    std::mutex blocks_mutex_;
    std::list<Block*> blocks_;


    std::atomic<bool> game_over_ = {true};
    std::atomic<bool> phase_running_ = {false};

    int dx_, dy_;
    int level_ = 1;
    Ball* main_ball_ = nullptr;
    ArrowHelper* main_arrow_helper_ = nullptr;

    int mouse_down_x_, mouse_down_y_;

public:
    Game(int width, int height);

    ~Game();

    void init();
    void gameStart(int level);
    void gameOver();
    bool isGameOver();
    bool checkGameOver();

    std::list<ImageRenderable*>* getObjectsPtr();

    void collisionWall();
    void collisionBlock();
    int nextLevel();

    void render(png_bytep* png_bytep_data);

    void fire();

    void onMouseLeftDown(const wxMouseEvent &e);
    void onMouseLeftUp(const wxMouseEvent &e);
    void onMouseMove(const wxMouseEvent &e);

    void checkCurrentPhaseOver();
};



#endif //GAME_H
