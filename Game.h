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
    const int width, height;
    int startX;
    const int ingameHeight = height - 140;
    int max_block_cnt_x = 6;
    int max_block_cnt_y = 8;

    int block_margin = 3;

    // todo://뭔지 모르겟으나 이따 화긴
    int startStage = 0;

    std::list<ImageRenderable*> objects;

    std::mutex ballsMutex;
    std::list<Ball*> balls;
    std::mutex blocksMutex;
    std::list<Block*> blocks;


    std::atomic<bool> phaseRunning = {false};

    int dx, dy;
    int level = 1;
    Ball* main_ball = nullptr;
    ArrowHelper* main_arrow_helper = nullptr;

    int mouse_down_x, mouse_down_y;

public:
    Game(int width, int height);

    ~Game();

    void gameStart(int level);
    void gameOver();
    bool isGameOver();

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
