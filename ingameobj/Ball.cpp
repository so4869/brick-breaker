//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "Ball.h"
#include <cmath>

// img: height x width x 4 (RGBA)인 평면 버퍼라고 가정
//      img.size() == height * width * 4
void drawFilledCircleRGBA(
    png_bytep* img,
    const int width, const int height,
    const int cx, const int cy, const int r,
    const unsigned char R,
    const unsigned char G,
    const unsigned char B,
    const unsigned char A)
{
    if (r <= 0) return;

    const float rf  = static_cast<float>(r);
    const float cxf = static_cast<float>(cx);
    const float cyf = static_cast<float>(cy);

    // 안티앨리어싱 경계용 반경 ±0.5
    const float rMinus  = rf - 0.5f;
    const float rPlus   = rf + 0.5f;
    const float rMinus2 = rMinus * rMinus;
    const float rPlus2  = rPlus  * rPlus;

    // bounding box (조금 여유)
    int y_min = static_cast<int>(std::floor(cy - r - 1.0f));
    int y_max = static_cast<int>(std::ceil (cy + r + 1.0f));
    int x_min = static_cast<int>(std::floor(cx - r - 1.0f));
    int x_max = static_cast<int>(std::ceil (cx + r + 1.0f));

    y_min = std::max(y_min, 0);
    y_max = std::min(y_max, height - 1);
    x_min = std::max(x_min, 0);
    x_max = std::min(x_max, width  - 1);

    const float i255 = 1.0f / 255.0f;

    const float srcR = R * i255;
    const float srcG = G * i255;
    const float srcB = B * i255;
    const float baseSrcA = A * i255;

    for (int y = y_min; y <= y_max; ++y) {
        float fy = static_cast<float>(y) + 0.5f;
        float dy = fy - cyf;

        for (int x = x_min; x <= x_max; ++x) {
            float fx = static_cast<float>(x) + 0.5f;
            float dx = fx - cxf;

            float dist2 = dx * dx + dy * dy;

            float coverage;

            // 1) 안쪽 깊숙이 → coverage = 1 (sqrt 없음)
            if (dist2 <= rMinus2) {
                coverage = 1.0f;
            }
            // 2) 바깥 멀리 → 그리지 않음 (sqrt 없음)
            else if (dist2 >= rPlus2) {
                continue;
            }
            // 3) 경계 영역만 sqrt 사용
            else {
                float dist = std::sqrt(dist2);  // 여기서만 sqrt 콜
                float t = rf - dist;            // r - dist ∈ (-0.5, 0.5)
                coverage = t + 0.5f;            // 0 ~ 1
                if (coverage <= 0.0f) continue;
            }

            float srcA = baseSrcA * coverage;
            if (srcA <= 0.0f) continue;

            png_bytep px = &(img[y][x * 4]);

            float dstR = px[0] * i255;
            float dstG = px[1] * i255;
            float dstB = px[2] * i255;
            float dstA = px[3] * i255;

            // source-over blending
            float outA = srcA + dstA * (1.0f - srcA);

            float outR, outG, outB;
            if (outA > 0.0f) {
                float dstFactor = dstA * (1.0f - srcA);
                outR = (srcR * srcA + dstR * dstFactor) / outA;
                outG = (srcG * srcA + dstG * dstFactor) / outA;
                outB = (srcB * srcA + dstB * dstFactor) / outA;
            } else {
                outR = outG = outB = 0.0f;
            }

            px[0] = static_cast<unsigned char>(outR * 255.0f + 0.5f);
            px[1] = static_cast<unsigned char>(outG * 255.0f + 0.5f);
            px[2] = static_cast<unsigned char>(outB * 255.0f + 0.5f);
            px[3] = static_cast<unsigned char>(outA * 255.0f + 0.5f);
        }
    }
}


Ball::Ball(const int center_x, const int center_y, const double radius,
    const unsigned char R, const unsigned char G, const unsigned char B, const unsigned char A):
        ImageRenderable(R, G, B, A), radius(radius) {

    this->point.set_xy(center_x, center_y);
}


void Ball::render(const int max_width, const int max_height, png_bytep *&png_bytep_data) {
    // drawFilledCircleRGBA(png_bytep_data, max_width, max_height, this->point.get_x(), this->point.get_y(),
    //     static_cast<int>(this->radius), this->R, this->G, this->B, this->A);
    // drawFilledCircleRGBA(png_bytep_data, max_width, max_height, this->point.get_x(), this->point.get_y(),
    //     static_cast<int>(this->radius * 0.8), 255, 255, 255, 255);
    // drawFilledCircleRGBA(png_bytep_data, max_width, max_height, this->point.get_x(), this->point.get_y(),
    //     static_cast<int>(this->radius * 0.5), this->R, this->G, this->B, this->A);

    drawFilledCircleRGBA(png_bytep_data, max_width, max_height, this->point.get_x(), this->point.get_y(),
        static_cast<int>(this->radius), this->R, this->G, this->B, this->A);
}

void Ball::setNum(const int num) {
    this->num = num;
}