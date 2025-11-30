//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>
#include <wx/statbmp.h>

class MyApp;

// wxWidgets Frame: 메모리에서 만든 PNG를 바로 wxImage로 읽어와서 띄움
class MyFrame : public wxFrame {
private:
    int width, height;
    wxStaticBitmap* bitmap_;
    wxTimer renderTimer;

    std::vector<unsigned char> *pngData = nullptr;
    std::mutex *pngMutex = nullptr;

    void OnTimer(wxTimerEvent &event);

public:
    // MyFrame(MyApp *my_app);
    MyFrame(int width, int height);
    void RenderImage(const std::vector<unsigned char> *pngData);
    void SetPngDataPtr(std::vector<unsigned char> *data, std::mutex *mutex);
    void bindEvent(MyApp &my_app);
};



#endif //MYFRAME_H
