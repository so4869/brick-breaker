//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "MyFrame.h"

#include "MyApp.h"
#include <wx/mstream.h>
#include <wx/event.h>

MyFrame::MyFrame(const int width, const int height)
        : wxFrame(nullptr, wxID_ANY, "brick-breaker", wxDefaultPosition,
            wxSize(width, height)), width(width), height(height),
            renderTimer(this)
{
    bitmap_ = new wxStaticBitmap(this, wxID_ANY, wxImage());
    // bmp->SetBitmap(image);
    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(bitmap_, 1, wxEXPAND | wxALL, 0);
    this->SetSizer(sizer);

    this->Bind(wxEVT_TIMER, &MyFrame::OnTimer, this);
    this->renderTimer.Start(1000 / 120);

    // bitmap_->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &e) {
    //     bitmap_->CaptureMouse();
    //     std::cout << "mouse down" << std::endl;
    // });
    // bitmap_->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent &e) {
    //     std::cout << "mouse up" << std::endl;
    //     if (bitmap_->HasCapture()) {
    //         bitmap_->ReleaseMouse();
    //     }
    // });

    // bitmap_->Bind(wxEVT_MOTION, [this](wxMouseEvent &e) {
    //     if (e.Dragging() && e.LeftIsDown()) {
    //         wxPoint p = e.GetPosition();
    //         std::cout << p.x << ", " << p.y << std::endl;
    //     }
    // });
}

void MyFrame::RenderImage(const std::vector<unsigned char> *pngData) {

    if (!this->IsShown()) {
        std::cout << "It is not shown yet. Therefore render will be skipped." << std::endl;
        return;
    }

    // 메모리 버퍼 → wxMemoryInputStream → wxImage(PNG)
    wxMemoryInputStream memStream(pngData->data(), pngData->size());
    const wxImage image(memStream, wxBITMAP_TYPE_PNG);

    if (!image.IsOk()) {
        wxMessageBox("wxImage 로딩 실패", "Error",
                     wxOK | wxICON_ERROR, this);
        return;
    }

    bitmap_->SetBitmap(image);
}

void MyFrame::SetPngDataPtr(std::vector<unsigned char> *data, std::mutex *mutex) {
    this->pngData = data;
    this->pngMutex = mutex;
}


void MyFrame::bindEvent(MyApp &my_app) {
    // bitmap_->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &e) {
    //     bitmap_->CaptureMouse();
    //     std::cout << "mouse down" << std::endl;
    // });
    // bitmap_->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent &e) {
    //     std::cout << "mouse up" << std::endl;
    //     if (bitmap_->HasCapture()) {
    //         bitmap_->ReleaseMouse();
    //     }
    // });
    // bitmap_->Bind(wxEVT_MOTION, [this](const wxMouseEvent &e) {
    //     if (e.Dragging() && e.LeftIsDown()) {
    //         wxPoint p = e.GetPosition();
    //         std::cout << p.x << ", " << p.y << std::endl;
    //     }
    // });


    bitmap_->Bind(wxEVT_LEFT_DOWN, [this, &my_app](wxMouseEvent &e) {
        bitmap_->CaptureMouse();
        // std::cout << "mouse down" << std::endl;
        my_app.mouseLeftDownEventListener(e);
    });
    bitmap_->Bind(wxEVT_LEFT_UP, [this, &my_app](wxMouseEvent &e) {
        // std::cout << "mouse up" << std::endl;
        if (bitmap_->HasCapture()) {
            bitmap_->ReleaseMouse();
        }
        my_app.mouseLeftUpEventListener(e);
    });
    this->bitmap_->Bind(wxEVT_MOTION, [this, &my_app](const wxMouseEvent &e) {
        my_app.mouseMotionEventListener(e);
    });
}



void MyFrame::OnTimer(wxTimerEvent &event) {
    // skip if data not ready.
    if (!this->pngData || !this->pngMutex) {
        return;
    }

    // // lock
    // std::lock_guard lock(*this->pngMutex);
    //
    // // skip if data not ready.
    // if (this->pngData->empty()) {
    //     return;
    // }
    //
    // RenderImage(this->pngData);

    // // lock shared data
    // std::vector<unsigned char> pngDataCopy;
    // {
    //     std::lock_guard lock(*this->pngMutex);
    //
    //     // skip if data not ready.
    //     if (this->pngData->empty()) {
    //         return;
    //     }
    //
    //     // 복사본 생성
    //     pngDataCopy = *this->pngData;
    // }
    //
    // // wxCallAfter로 GUI 안전한 방식으로 업데이트
    // this->CallAfter([this, pngDataCopy]() {
    //     std::lock_guard lock(*this->pngMutex);
    //     RenderImage(&pngDataCopy);
    // })

    // wxCallAfter로 GUI 안전한 방식으로 업데이트
    this->CallAfter([this]() {
        std::lock_guard lock(*this->pngMutex);
        RenderImage(this->pngData);
    });
}
