//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "MyApp.h"

#include <cstring>

#include "imageutil.hpp"

void MyApp::consist_png() {
    // create_png(width, height, rect_x, rect_y, rect_x + rect_size_x, rect_y + rect_size_y, pngDataBuffer);
    int const row_bytes = this->width * this->bytes_per_pixel;

    for (int y = 0; y < this->height; ++y) {
        // memory상에 rgba가 255, 255, 255, 255로 드감 0xFF로 한번에 조진다.
        std::memset(this->png_bytep_data[y], 0xFF, row_bytes);
    }


    this->game->render(this->png_bytep_data);
    copy_png(this->width, this->height, this->png_bytep_data, this->pngDataBuffer);

    {
        std::lock_guard lock(this->pngMutex);
        this->pngData.swap(this->pngDataBuffer);
    }
}


bool MyApp::OnInit() {
    // memory allocate, for libpng
    this->png_bytep_data = static_cast<png_bytep *>(std::malloc(sizeof(png_bytep) * this->height));
    for (int y = 0; y < this->height; ++y) {
        this->png_bytep_data[y] = static_cast<png_bytep>(std::malloc(this->width * this->bytes_per_pixel));
    }

    // enable png handler
    wxInitAllImageHandlers();
    this->frame = new MyFrame(this->width, this->height);
    // pngData와 pngMutex에 대한 참조 제공, multi thread 고려
    this->frame->SetPngDataPtr(&this->pngData, &this->pngMutex);

    // start
    // ball, #5ba8f4
    this->game = new Game(this->width, this->height);
    this->game->gameStart(1);

    // thread 시작, sleep time for 120 fps
    this->image_render_thread = std::thread([this]() {
        while (!this->stopFlag.load()) {
            consist_png();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 120));
        }
    });

    // thread 시작, sleep time for 120 fps
    this->background_thread = std::thread([this]() {
        while (!this->stopFlag.load()) {
            // this->game->collisionWall();
            // this->game->collisionBlock();
            this->game->checkCurrentPhaseOver();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 120));
        }
    });

    this->frame->bindEvent(*this);
    this->frame->Show(true);

    return true;
}

int MyApp::OnExit() {
    // set stop flag to true to stop thread
    this->stopFlag.store(true);
    // join render thread to main thread if possible
    if (this->image_render_thread.joinable()) {
        this->image_render_thread.join();
    }
    // join render thread to main thread if possible
    if (this->background_thread.joinable()) {
        this->background_thread.join();
    }

    // release png_bytep data
    for (int y = 0; y < this->height; ++y) {
        std::free(this->png_bytep_data[y]);
    }
    std::free(this->png_bytep_data);

    // release game object
    delete this->game;

    return 1;
}

int MyApp::getWidth() {
    return this->width;
}

int MyApp::getHeight() {
    return this->height;
}

void MyApp::mouseMotionEventListener(const wxMouseEvent &e) {
    // if (e.Dragging() && e.LeftIsDown()) {
    //     // const wxPoint p = e.GetPosition();
    //     // std::cout << p.x << ", " << p.y << std::endl;
    // }

    // std::cout << "asdfa testEventListener" << this->getWidth() << std::endl;
    this->game->onMouseMove(e);
}

void MyApp::mouseLeftDownEventListener(const wxMouseEvent &e) {
    this->game->onMouseLeftDown(e);
}

void MyApp::mouseLeftUpEventListener(const wxMouseEvent &e) {
    this->game->onMouseLeftUp(e);
}
