//
// Created by HyunJin SO on 2025. 11. 30..
//

#include "Block.h"

#include <string>
#include <algorithm>

// -----------------------------------------
// 픽셀 찍기
// -----------------------------------------
inline void putPixelRGBA(
    png_bytep* img,
    int width, int height,
    int x, int y,
    unsigned char R,
    unsigned char G,
    unsigned char B,
    unsigned char A)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    png_bytep px = &(img[y][x * 4]);
    px[0] = R;
    px[1] = G;
    px[2] = B;
    px[3] = A;
}

// -----------------------------------------
// 5×7 숫자 비트맵 폰트
// -----------------------------------------
static const unsigned char DIGITS[10][7] = {
    {0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110}, // 0
    {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}, // 1
    {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111}, // 2
    {0b01110, 0b10001, 0b00001, 0b00110, 0b00001, 0b10001, 0b01110}, // 3
    {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010}, // 4
    {0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110}, // 5
    {0b00110, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110}, // 6
    {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000}, // 7
    {0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110}, // 8
    {0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00010, 0b01100}  // 9
};


// -----------------------------------------
// 단일 숫자 렌더링 (scale 적용)
// -----------------------------------------
void drawDigitScaled(
    png_bytep* img,
    int width, int height,
    int x, int y,
    int digit,
    int scale,
    unsigned char R,
    unsigned char G,
    unsigned char B,
    unsigned char A)
{
    if (digit < 0 || digit > 9) return;

    const unsigned char* bm = DIGITS[digit];

    const int digitW = 5;
    const int digitH = 7;

    for (int row = 0; row < digitH; ++row) {
        unsigned char bits = bm[row];
        for (int col = 0; col < digitW; ++col) {
            if (bits & (1 << (4 - col))) {
                // scale 배로 픽셀 확장
                for (int dy = 0; dy < scale; ++dy) {
                    for (int dx = 0; dx < scale; ++dx) {
                        putPixelRGBA(
                            img, width, height,
                            x + col * scale + dx,
                            y + row * scale + dy,
                            R, G, B, A
                        );
                    }
                }
            }
        }
    }
}

// -----------------------------------------
// 여러 자리 숫자: 중앙 정렬 + scale 적용
// -----------------------------------------
void drawNumberCenteredScaled(
    png_bytep* img,
    int maxWidth, int maxHeight,
    int rectX, int rectY,
    int rectW, int rectH,
    int number,
    int scale,
    unsigned char R, unsigned char G,
    unsigned char B, unsigned char A)
{
    std::string s = std::to_string(number);

    const int digitW = 5 * scale;
    const int digitH = 7 * scale;
    const int spacing = 1 * scale;

    int totalWidth = s.size() * digitW + (s.size() - 1) * spacing;
    int totalHeight = digitH;

    // 중앙 배치
    int startX = rectX + (rectW - totalWidth) / 2;
    int startY = rectY + (rectH - totalHeight) / 2;

    // 숫자 렌더링
    int cursorX = startX;
    for (char ch : s) {
        drawDigitScaled(
            img,
            maxWidth,
            maxHeight,
            cursorX,
            startY,
            ch - '0',
            scale,
            R, G, B, A
        );
        cursorX += digitW + spacing;
    }
}

// -----------------------------------------
// 직사각형 + 중앙 숫자(스케일 지원)
// -----------------------------------------
void drawRectangleWithNumber(
    png_bytep* img,
    const int maxWidth, const int maxHeight,
    const int cx, const int cy,
    const int width, const int height,
    const int number,
    const int scale,
    const unsigned char R,
    const unsigned char G,
    const unsigned char B,
    const unsigned char A)
{
    // 직사각형 채우기
    for (int y = std::max(0, cy); y <= std::min(cy + height - 1, maxHeight - 1); ++y) {
        for (int x = std::max(0, cx); x <= std::min(cx + width - 1, maxWidth - 1); ++x) {
            png_bytep px = &(img[y][x * 4]);
            px[0] = R;
            px[1] = G;
            px[2] = B;
            px[3] = A;
        }
    }

    // 중앙 숫자 (스케일 적용)
    drawNumberCenteredScaled(
        img,
        maxWidth, maxHeight,
        cx, cy,
        width, height,
        number,
        scale,
        255, 255, 255, 255  // 흰색 텍스트
    );
}



// void drawRectangleWithNumber(
//     png_bytep* img,
//     const int maxWidth, const int maxHeight,
//     const int cx, const int cy,
//     const int width, const int height,
//     const int number,
//     const unsigned char R,
//     const unsigned char G,
//     const unsigned char B,
//     const unsigned char A)
// {
//
//     for (int y = std::max(0, cy); y <= std::min(cy + height, maxHeight); ++y) {
//         for (int x = std::max(0, cx); x <= std::min(cx + width, maxWidth); ++x) {
//             png_bytep px = &(img[y][x * 4]);
//
//             px[0] = static_cast<unsigned char>(R);
//             px[1] = static_cast<unsigned char>(G);
//             px[2] = static_cast<unsigned char>(B);
//             px[3] = static_cast<unsigned char>(A);
//         }
//     }
// }

Block::Block(int x, int y, int width, int height, int maxHp, int hp, unsigned char R, unsigned char G, unsigned char B):
    ImageRenderable(R, G, B, 255), width(width), height(height), maxHp(maxHp), hp(hp){
    this->setR2Point(x, y);
    this->A = hp / maxHp * 255;
}

void Block::render(int max_width, int max_height, png_bytep *&png_vec) {
    drawRectangleWithNumber(png_vec, max_width, max_height, point.get_x(), point.get_y(),  width, height, hp, 2, R, G, B, A);
}

