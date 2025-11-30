#include <iostream>
#include <setjmp.h>
#include <cmath>
#include <algorithm>
#include <png.h>

#include "imageutil.hpp"

#include <vector>

// libpng가 쓰는 메모리 버퍼
struct PngBuffer {
    std::vector<unsigned char> data;
};

// libpng custom write callback to write on memory not disk.s
void png_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    auto* pngBuf = static_cast<PngBuffer*>(png_get_io_ptr(png_ptr));
    pngBuf->data.insert(pngBuf->data.end(), data, data + length);
}

void png_flush(png_structp png_ptr) {
    // 아무 것도 안 해도 됨
    (void)png_ptr;
}

// libpng로 8bit RGBA 이미지(흰 배경 + 빨간 사각형)를 PNG 포맷 메모리 버퍼로 인코딩
bool create_png(int width, int height,
                          int rect_x1, int rect_y1,
                          int rect_x2, int rect_y2,
                          std::vector<unsigned char>& outPng)
{
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)nullptr);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        // libpng 내부 에러
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    PngBuffer pngBuf;
    png_set_write_fn(png_ptr, &pngBuf, png_write, png_flush);

    // 8bit RGBA
    png_set_IHDR(png_ptr, info_ptr,
                 width, height,
                 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    const int bytes_per_pixel = 4;
    png_bytep* row_pointers =
        (png_bytep*)std::malloc(sizeof(png_bytep) * height);
    if (!row_pointers) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    bool ok = true;

    for (int y = 0; y < height; ++y) {
        row_pointers[y] = (png_bytep)std::malloc(width * bytes_per_pixel);
        if (!row_pointers[y]) {
            ok = false;
            for (int k = 0; k < y; ++k) {
                std::free(row_pointers[k]);
            }
            std::free(row_pointers);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return false;
        }

        for (int x = 0; x < width; ++x) {
            png_bytep px = &(row_pointers[y][x * bytes_per_pixel]);

            // 기본 배경: 흰색
            px[0] = 255; // R
            px[1] = 255; // G
            px[2] = 255; // B
            px[3] = 255; // A

            // 사각형 영역: 빨간색
            if (x >= rect_x1 && x <= rect_x2 &&
                y >= rect_y1 && y <= rect_y2)
            {
                px[0] = 255; // R
                px[1] = 255;   // G
                px[2] = 0;   // B
                px[3] = 255; // A
                // std::cout << "rect_y2: " << rect_y2 << std::endl;
            }
        }
    }

    if (ok) {
        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, nullptr);
    }

    for (int y = 0; y < height; ++y) {
        std::free(row_pointers[y]);
    }
    std::free(row_pointers);

    if (!ok) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    // 최종 PNG 바이트 배열 복사
    outPng = std::move(pngBuf.data);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    return true;
}


png_bytep* create_png_bytep_ptr(const int width, const int height)
{
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return nullptr;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)nullptr);
        return nullptr;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        // libpng 내부 에러
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return nullptr;
    }

    PngBuffer pngBuf;
    png_set_write_fn(png_ptr, &pngBuf, png_write, png_flush);

    // 8bit RGBA
    png_set_IHDR(png_ptr, info_ptr,
                 width, height,
                 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    const int bytes_per_pixel = 4;
    png_bytep* row_pointers = static_cast<png_bytep *>(std::malloc(sizeof(png_bytep) * height));
    if (!row_pointers) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return nullptr;
    }

    bool ok = true;

    for (int y = 0; y < height; ++y) {
        row_pointers[y] = static_cast<png_bytep>(std::malloc(width * bytes_per_pixel));
        if (!row_pointers[y]) {
            ok = false;
            for (int k = 0; k < y; ++k) {
                std::free(row_pointers[k]);
            }

            std::free(row_pointers);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return nullptr;
        }
    }

    return row_pointers;
}


/**
 *
 * @param width image의 width
 * @param height image의 height
 * @param row_pointers create_png_bytep_ptr로 만들어진 row_pointers, 동일한 width, height값으로 만들어져야한다.
 * @param outPng
 * @return succeeded
 */
bool copy_png(const int width, const int height, png_bytep* row_pointers, std::vector<unsigned char> &outPng)
{
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)nullptr);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        // libpng 내부 에러
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    PngBuffer pngBuf;
    png_set_write_fn(png_ptr, &pngBuf, png_write, png_flush);

    // 8bit RGBA
    png_set_IHDR(png_ptr, info_ptr,
                 width, height,
                 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    if (!row_pointers) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, nullptr);

    // 최종 PNG 바이트 배열 복사
    outPng = std::move(pngBuf.data);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    return true;
}


// 0~1 coverage 기반 RGBA 블렌딩
inline void blendPixel(
    png_bytep px,
    unsigned char R, unsigned char G, unsigned char B, unsigned char A,
    float coverage
) {
    if (coverage <= 0.0f) return;
    if (coverage > 1.0f) coverage = 1.0f;

    // src, dst alpha (0~1)
    float srcA = (A / 255.0f) * coverage;
    if (srcA <= 0.0f) return;

    float dstA = px[3] / 255.0f;

    float outA = srcA + dstA * (1.0f - srcA);
    if (outA <= 0.0f) {
        px[0] = px[1] = px[2] = px[3] = 0;
        return;
    }

    auto chanBlend = [&](unsigned char dstC, unsigned char srcC) -> unsigned char {
        float s = (srcC / 255.0f) * srcA;
        float d = (dstC / 255.0f) * dstA * (1.0f - srcA);
        float o = (s + d) / outA;
        if (o < 0.0f) o = 0.0f;
        if (o > 1.0f) o = 1.0f;
        return static_cast<unsigned char>(o * 255.0f + 0.5f);
    };

    unsigned char outR = chanBlend(px[0], R);
    unsigned char outG = chanBlend(px[1], G);
    unsigned char outB = chanBlend(px[2], B);
    unsigned char outAlpha = static_cast<unsigned char>(outA * 255.0f + 0.5f);

    px[0] = outR;
    px[1] = outG;
    px[2] = outB;
    px[3] = outAlpha;
}

// 안티앨리어싱된 두꺼운 화살표 그리기
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
) {
    if (thickness <= 0.0f) return;

    float dx = x1 - x0;
    float dy = y1 - y0;
    const float len2 = dx * dx + dy * dy;
    if (len2 <= 1e-6f) return;

    float len    = std::sqrt(len2);
    if (len > max_len) {
        float scale = max_len / len;
        dx *= scale;
        dy *= scale;
        x1 = x0 + dx;
        y1 = y0 + dy;
        len = max_len;
    }

    const float invLen = 1.0f / len;

    const float dir_x = dx * invLen;
    const float dir_y = dy * invLen;
    const float n_x   = -dir_y;
    const float n_y   =  dir_x;

    const float half_thick = thickness * 0.5f;
    const int   BPP        = 4;

    // ---- (1) 화살촉 밑변 위치 계산 ----
    float shaft_len = len - arrow_len;
    if (shaft_len < 0.0f) shaft_len = 0.0f;   // 방어

    float base_x = x0 + dir_x * shaft_len;
    float base_y = y0 + dir_y * shaft_len;

    // ---- (2) 선분(shaft): x0,y0 ~ base_x,base_y 만 그리기 (AA+블렌딩) ----
    float min_xf = std::min(x0, base_x) - (half_thick + 1.0f);
    float max_xf = std::max(x0, base_x) + (half_thick + 1.0f);
    float min_yf = std::min(y0, base_y) - (half_thick + 1.0f);
    float max_yf = std::max(y0, base_y) + (half_thick + 1.0f);

    int min_x = std::max(0,          (int)std::floor(min_xf));
    int max_x = std::min(width  - 1, (int)std::ceil (max_xf));
    int min_y = std::max(0,          (int)std::floor(min_yf));
    int max_y = std::min(height - 1, (int)std::ceil (max_yf));

    for (int y = min_y; y <= max_y; ++y) {
        png_bytep row = png_vec[y];
        float py = (float)y + 0.5f;

        for (int x = min_x; x <= max_x; ++x) {
            float px = (float)x + 0.5f;

            float vx = px - x0;
            float vy = py - y0;

            float u = vx * dir_x + vy * dir_y;
            if (u < 0.0f || u > shaft_len) continue;

            float w = vx * n_x + vy * n_y;
            float dist_to_edge = std::fabs(w) - half_thick;

            float coverage;
            if (dist_to_edge <= -0.5f) {
                coverage = 1.0f;
            } else if (dist_to_edge >= 0.5f) {
                continue;
            } else {
                coverage = 0.5f - dist_to_edge; // -0.5 → 1, +0.5 → 0
            }

            png_bytep p = row + x * BPP;
            blendPixel(p, R, G, B, A, coverage);
        }
    }

    // ---- (3) 화살촉: base_x,base_y ~ x1,y1 삼각형 ----
    float tip_x = x1;
    float tip_y = y1;

    float left_x  = base_x + n_x * (arrow_width * 0.5f);
    float left_y  = base_y + n_y * (arrow_width * 0.5f);
    float right_x = base_x - n_x * (arrow_width * 0.5f);
    float right_y = base_y - n_y * (arrow_width * 0.5f);

    // bounding box
    float axmin_f = std::min({tip_x, left_x, right_x}) - 1.0f;
    float axmax_f = std::max({tip_x, left_x, right_x}) + 1.0f;
    float aymin_f = std::min({tip_y, left_y, right_y}) - 1.0f;
    float aymax_f = std::max({tip_y, left_y, right_y}) + 1.0f;

    int ax0 = std::max(0,          (int)std::floor(axmin_f));
    int ax1 = std::min(width  - 1, (int)std::ceil (axmax_f));
    int ay0 = std::max(0,          (int)std::floor(aymin_f));
    int ay1 = std::min(height - 1, (int)std::ceil (aymax_f));

    auto edge = [](float x, float y,
                   float x0, float y0,
                   float x1, float y1) -> float {
        return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
    };

    // 중심점에서 내부 부호 방향 결정
    float cx = (tip_x + left_x + right_x) / 3.0f;
    float cy = (tip_y + left_y + right_y) / 3.0f;
    float c0 = edge(cx, cy, tip_x,  tip_y,  left_x,  left_y );
    float c1 = edge(cx, cy, left_x, left_y, right_x, right_y);
    float c2 = edge(cx, cy, right_x,right_y,tip_x,  tip_y  );
    bool insidePositive = (c0 >= 0.0f && c1 >= 0.0f && c2 >= 0.0f);

    // 엣지 벡터 길이 (signed distance용)
    float e0_x = left_x  - tip_x;
    float e0_y = left_y  - tip_y;
    float e1_x = right_x - left_x;
    float e1_y = right_y - left_y;
    float e2_x = tip_x   - right_x;
    float e2_y = tip_y   - right_y;

    float invLen0 = 1.0f / std::sqrt(e0_x*e0_x + e0_y*e0_y);
    float invLen1 = 1.0f / std::sqrt(e1_x*e1_x + e1_y*e1_y);
    float invLen2 = 1.0f / std::sqrt(e2_x*e2_x + e2_y*e2_y);

    for (int y = ay0; y <= ay1; ++y) {
        png_bytep row = png_vec[y];
        float py = (float)y + 0.5f;

        for (int x = ax0; x <= ax1; ++x) {
            float px = (float)x + 0.5f;

            float e0 = edge(px, py, tip_x,  tip_y,  left_x,  left_y );
            float e1 = edge(px, py, left_x, left_y, right_x, right_y);
            float e2 = edge(px, py, right_x,right_y,tip_x,  tip_y  );

            if (!insidePositive) { e0 = -e0; e1 = -e1; e2 = -e2; }

            // signed distance(>0: 내부, <0: 외부)
            float d0 = e0 * invLen0;
            float d1 = e1 * invLen1;
            float d2 = e2 * invLen2;
            float mind = std::min(d0, std::min(d1, d2));

            if (mind <= -0.5f) {
                // 완전 외부
                continue;
            }

            png_bytep p = row + x * BPP;

            if (mind >= 0.5f) {
                // 충분히 내부 → 완전 불투명 오버라이트
                p[0] = R;
                p[1] = G;
                p[2] = B;
                p[3] = A;
            } else {
                // 경계 ±0.5px 구간만 AA (coverage 0~1)
                float coverage = mind + 0.5f; // mind=-0.5→0, mind=0.5→1
                if (coverage < 0.0f) coverage = 0.0f;
                if (coverage > 1.0f) coverage = 1.0f;

                // 색만 부드럽게, 알파는 항상 255(불투명)
                float dstR = p[0] / 255.0f;
                float dstG = p[1] / 255.0f;
                float dstB = p[2] / 255.0f;

                float srcR = R / 255.0f;
                float srcG = G / 255.0f;
                float srcB = B / 255.0f;

                float outR = coverage * srcR + (1.0f - coverage) * dstR;
                float outG = coverage * srcG + (1.0f - coverage) * dstG;
                float outB = coverage * srcB + (1.0f - coverage) * dstB;

                p[0] = static_cast<unsigned char>(outR * 255.0f + 0.5f);
                p[1] = static_cast<unsigned char>(outG * 255.0f + 0.5f);
                p[2] = static_cast<unsigned char>(outB * 255.0f + 0.5f);
                p[3] = 255; // 항상 불투명
            }
        }
    }
}