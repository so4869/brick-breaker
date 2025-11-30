//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "ImageRenderable.h"

void ImageRenderable::setR2Point(const int x, const int y) {
    this->point.set_xy(x, y);
}

bool ImageRenderable::getSkipRender() {
    return this->skipRender;
}

void ImageRenderable::setSkipRender(const bool skipRender) {
    this->skipRender = skipRender;
}