//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef GAME_H
#define GAME_H
#include <list>
#include <wx/event.h>

#include "ingameobj/ArrowHelper.h"
#include "ingameobj/Ball.h"
#include "ingameobj/Block.h"
#include "ingameobj/ImageRenderable.h"


class Game {
private:
    const int width, height;
    const int ingameHeight = height - 140;
    int max_block_cnt_x = 6;
    int max_block_cnt_y = 8;

    int block_margin = 3;

    std::vector<ImageRenderable*> objects;
    std::list<Block*> blocks;

    int level = 1;
    Ball* main_ball = nullptr;
    ArrowHelper* main_arrow_helper = nullptr;

    int mouse_down_x, mouse_down_y;

public:
    Game(int width, int height);

    ~Game();

    void gameStart(int level);
    void gameEnd();

    std::vector<ImageRenderable*>* getObjectsPtr();

    void render(png_bytep* png_bytep_data);

    void fire();

    void onMouseLeftDown(const wxMouseEvent &e);
    void onMouseLeftUp(const wxMouseEvent &e);
    void onMouseMove(const wxMouseEvent &e);
};



#endif //GAME_H
