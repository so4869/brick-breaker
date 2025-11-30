//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef GAME_H
#define GAME_H
#include <wx/event.h>

#include "ingameobj/ArrowHelper.h"
#include "ingameobj/ImageRenderable.h"


class Game {
private:
    const int width, height;
    std::vector<ImageRenderable*> objects;
    const int game_board[8][6] = {};
    ArrowHelper* main_arrow_helper = nullptr;

    int mouse_down_x, mouse_down_y;

public:
    Game(int width, int height);

    ~Game();

    void gameStart(int level);
    void gameEnd();

    std::vector<ImageRenderable*>* getObjectsPtr();

    void render(png_bytep* png_bytep_data);
    void onMouseLeftDown(const wxMouseEvent &e);
    void onMouseLeftUp(const wxMouseEvent &e);
    void onMouseMove(const wxMouseEvent &e);
};



#endif //GAME_H
