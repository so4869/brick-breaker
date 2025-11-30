//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "Game.h"
#include "ingameobj/Ball.h"

Game::Game(const int width, const int height): width(width), height(height) {
}

Game::~Game() {
    // release ImageRenderable objects
    for (ImageRenderable* &it : objects) {
        delete it;
    }
}


void Game::gameStart(const int level) {
    auto* main_ball = new Ball(50, 350, 6, 91, 168, 244, 255);
    main_arrow_helper = new ArrowHelper(50, 350, 100, 0, 91, 168, 244, 128);
    main_arrow_helper->setSkipRender(true);

    objects.push_back(main_ball);
    objects.push_back(main_arrow_helper);
}

void Game::gameEnd() {

}

std::vector<ImageRenderable*>* Game::getObjectsPtr() {
    return &this->objects;
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
}


void Game::onMouseLeftDown(const wxMouseEvent &e) {
    main_arrow_helper->setSkipRender(false);

    const auto p = e.GetPosition();
    this->mouse_down_x = p.x;
    this->mouse_down_y = p.y;
    // std::cout << "mouse down" << std::endl;
}

void Game::onMouseLeftUp(const wxMouseEvent &e) {
    main_arrow_helper->setSkipRender(true);
    // std::cout << "mouse up" << std::endl;
}

void Game::onMouseMove(const wxMouseEvent &e) {
    if (e.Dragging() && e.LeftIsDown()) {
        const wxPoint p = e.GetPosition();

        int dx = p.x - mouse_down_x;
        int dy = p.y - mouse_down_y;
        if (dx == 0) {
            return;
        }

        // double slope = static_cast<double>(dy) / dx;
        const double theta = -atan2(dy, dx);

        // std::cout << p.x << ", " << p.y << ", 기울기: " << slope << std::endl;

        main_arrow_helper->setTheta(theta);
    }
}


