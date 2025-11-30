//
// Created by HyunJin SO on 2025. 11. 27..
//

#include <cmath>

#include "ArrowHelper.h"
#include "../imageutil.hpp"

ArrowHelper::ArrowHelper(int center_x, int center_y, double radius, double theta,
    unsigned char R, unsigned char G, unsigned char B, unsigned char A): ImageRenderable(R, G, B, A),
        radius(radius), theta(theta) {
    this->point.set_xy(center_x, center_y);
}


void ArrowHelper::render(int max_width, int max_height, png_bytep* &png_vec) {
    const float x0 = this->point.get_x();
    const float y0 = this->point.get_y();
    const float x1 = x0 + radius * cos(theta);
    const float y1 = y0 - radius * sin(theta);

    drawThickArrowRGBA_AA(
        png_vec, max_width, max_height,
        x0, y0,      // start
        x1, y1,     // end
        4.1f,         // line thickness
        14.0f,        // arrow length
        14.0f,        // arrow width
        100.0f,
        R, G, B, A
    );
}

void ArrowHelper::setRadius(const double radius) {
    this->radius = radius;
}

void ArrowHelper::setTheta(const double theta) {
    this->theta = theta;
}
