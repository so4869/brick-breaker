//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "ImageRenderable.h"

void ImageRenderable::setPoint(const int x, const int y) {
    this->point.set_xy(x, y);
}

R2Point& ImageRenderable::getPoint() {
    return this->point;
}

bool ImageRenderable::getSkipRender() {
    return this->skipRender;
}

void ImageRenderable::setSkipRender(const bool skipRender) {
    this->skipRender = skipRender;
}

void ImageRenderable::setRGBA(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
    this->R = R;
    this->G = G;
    this->B = B;
    this->A = A;
}
