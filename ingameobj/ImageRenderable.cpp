//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "ImageRenderable.h"

void ImageRenderable::setPoint(const int x, const int y) {
    this->point.setXY(x, y);
}

R2Point& ImageRenderable::getPoint() {
    return this->point;
}

bool ImageRenderable::getSkipRender() {
    return this->skip_render_;
}

void ImageRenderable::setSkipRender(const bool skip_render) {
    this->skip_render_ = skip_render;
}

void ImageRenderable::setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    this->r_ = r;
    this->g_ = g;
    this->b_ = b;
    this->a_ = a;
}
