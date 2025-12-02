//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "MyApp.h"

#include <cstring>

#include "imageutil.hpp"

void MyApp::consistPng() {
    // create_png(width, height, rect_x, rect_y, rect_x + rect_size_x, rect_y + rect_size_y, pngDataBuffer);
    int const row_bytes = this->width_ * this->bytes_per_pixel_;

    for (int y = 0; y < this->height_; ++y) {
        // memory상에 rgba가 255, 255, 255, 255로 드감 0xFF로 한번에 조진다.
        std::memset(this->png_bytep_data_[y], 0xFF, row_bytes);
    }


    this->game_->render(this->png_bytep_data_);
    copy_png(this->width_, this->height_, this->png_bytep_data_, this->png_data_buffer_);

    {
        std::lock_guard lock(this->png_mutex_);
        this->png_data_.swap(this->png_data_buffer_);
    }
}


bool MyApp::OnInit() {
    // memory allocate, for libpng
    this->png_bytep_data_ = static_cast<png_bytep *>(std::malloc(sizeof(png_bytep) * this->height_));
    for (int y = 0; y < this->height_; ++y) {
        this->png_bytep_data_[y] = static_cast<png_bytep>(std::malloc(this->width_ * this->bytes_per_pixel_));
    }

    // enable png handler
    wxInitAllImageHandlers();
    this->frame_ = new MyFrame(this->width_, this->height_);
    // pngData와 pngMutex에 대한 참조 제공, multi thread 고려
    this->frame_->setPngDataPtr(&this->png_data_, &this->png_mutex_);

    // start
    // ball, #5ba8f4
    this->game_ = new Game(this->width_, this->height_);
    this->game_->gameStart(1);

    // thread 시작, sleep time for 120 fps
    this->image_render_thread_ = std::thread([this]() {
        while (!this->stop_flag_.load()) {
            consistPng();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 120));
        }
    });

    // thread 시작, sleep time for 120 fps
    this->background_thread_ = std::thread([this]() {
        while (!this->stop_flag_.load()) {
            // this->game->collisionWall();
            // this->game->collisionBlock();
            this->game_->checkCurrentPhaseOver();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 120));
        }
    });

    this->frame_->bindEvent(*this);
    this->frame_->Show(true);

    return true;
}

int MyApp::OnExit() {
    // set stop flag to true to stop thread
    this->stop_flag_.store(true);
    // join render thread to main thread if possible
    if (this->image_render_thread_.joinable()) {
        this->image_render_thread_.join();
    }
    // join render thread to main thread if possible
    if (this->background_thread_.joinable()) {
        this->background_thread_.join();
    }

    // release png_bytep data
    for (int y = 0; y < this->height_; ++y) {
        std::free(this->png_bytep_data_[y]);
    }
    std::free(this->png_bytep_data_);

    // release game object
    delete this->game_;

    return 1;
}

int MyApp::getWidth() {
    return this->width_;
}

int MyApp::getHeight() {
    return this->height_;
}

void MyApp::mouseMotionEventListener(const wxMouseEvent &e) {
    // if (e.Dragging() && e.LeftIsDown()) {
    //     // const wxPoint p = e.GetPosition();
    //     // std::cout << p.x << ", " << p.y << std::endl;
    // }

    // std::cout << "asdfa testEventListener" << this->getWidth() << std::endl;
    this->game_->onMouseMove(e);
}

void MyApp::mouseLeftDownEventListener(const wxMouseEvent &e) {
    this->game_->onMouseLeftDown(e);
}

void MyApp::mouseLeftUpEventListener(const wxMouseEvent &e) {
    this->game_->onMouseLeftUp(e);
}
