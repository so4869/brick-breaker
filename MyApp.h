//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef MYAPP_H
#define MYAPP_H

#include <atomic>
#include <wx/wx.h>
#include <vector>
#include <thread>

#include "MyFrame.h"
#include "Game.h"

class MyApp : public wxApp {
private:
    std::thread image_render_thread_;
    std::thread background_thread_;
    std::atomic<bool> stop_flag_ = {false};

    std::mutex png_mutex_;
    std::vector<unsigned char> png_data_;
    std::vector<unsigned char> png_data_buffer_;
    png_bytep *png_bytep_data_ = nullptr;

    MyFrame *frame_ = nullptr;
    Game *game_ = nullptr;

    const int width_ = 400, height_ = 500;
    const int bytes_per_pixel_ = 4;

    // std::vector<ImageRenderable*> objects;

    void consistPng();

public:
    bool OnInit() override;
    int OnExit() override;
    int getWidth();
    int getHeight();

    void mouseMotionEventListener(const wxMouseEvent &event);
    void mouseLeftDownEventListener(const wxMouseEvent &event);
    void mouseLeftUpEventListener(const wxMouseEvent &event);
};



#endif //MYAPP_H
