//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef MYFRAME_H
#define MYFRAME_H

#include <mutex>
#include <vector>
#include <wx/wx.h>
#include <wx/statbmp.h>

class MyApp;

// wxWidgets Frame: 메모리에서 만든 PNG를 바로 wxImage로 읽어와서 띄움
class MyFrame : public wxFrame {
private:
    int width_, height_;
    wxStaticBitmap* bitmap_;
    wxTimer render_timer_;

    std::vector<unsigned char> *png_data_ = nullptr;
    std::mutex *png_mutex_ = nullptr;

    void OnTimer(wxTimerEvent &event);

public:
    // MyFrame(MyApp *my_app);
    MyFrame(int width, int height);
    void renderImage(const std::vector<unsigned char> *png_data);
    void setPngDataPtr(std::vector<unsigned char> *data, std::mutex *mutex);
    void bindEvent(MyApp &my_app);
};



#endif //MYFRAME_H
