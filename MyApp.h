//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef MYAPP_H
#define MYAPP_H

#include <wx/wx.h>
#include <vector>
#include <thread>

#include "MyFrame.h"
#include "Game.h"

class MyApp : public wxApp {
private:
    std::thread image_render_thread;
    std::atomic<bool> stopFlag = {false};

    std::mutex pngMutex;
    std::vector<unsigned char> pngData;
    std::vector<unsigned char> pngDataBuffer;
    png_bytep *png_bytep_data = nullptr;

    MyFrame *frame = nullptr;
    Game *game = nullptr;

    const int width = 400, height = 500;
    const int bytes_per_pixel = 4;

    // std::vector<ImageRenderable*> objects;

    void consist_png();

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
