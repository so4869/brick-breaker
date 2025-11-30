#pragma once

#include <setjmp.h>   // jmp_buf 정의
extern "C" {
#include <png.h>      // libpng
}

// libpng custom write callback to write on memory not disk.s
void png_write(png_structp png_ptr, png_bytep data, png_size_t length);

void png_flush(png_structp png_ptr);

// libpng로 8bit RGBA 이미지를 PNG 포맷 메모리 버퍼로 인코딩
bool create_png(int width, int height,
                  int rect_x1, int rect_y1,
                  int rect_x2, int rect_y2,
                  std::vector<unsigned char>& outPng);

png_bytep* create_png_bytep_ptr(int width, int height);
bool copy_png(int width, int height, png_bytep* row_pointers, std::vector<unsigned char> &outPng);



void drawThickArrowRGBA_AA(
    png_bytep* &png_vec,
    int width,
    int height,
    float x0, float y0,      // 시작점
    float x1, float y1,      // 끝점 (화살촉 방향)
    float thickness,         // 선 두께 (px)
    float arrow_len,         // 화살촉 길이 (px)
    float arrow_width,       // 화살촉 밑변 너비 (px)
    float max_len,
    unsigned char R,
    unsigned char G,
    unsigned char B,
    unsigned char A
);

inline void blendPixel(
    png_bytep px,
    unsigned char R, unsigned char G, unsigned char B, unsigned char A,
    float coverage
);