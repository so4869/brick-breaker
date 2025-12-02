//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <cstdio>
#include <png.h>
#include <pngconf.h>
#include <vector>

#include "R2Point.h"


class ImageRenderable {
protected:
    R2Point point;
    unsigned char r_ = 0, g_ = 0, b_ = 0, a_ = 0;
    bool skip_render_ = false;
public:
    ImageRenderable() = default;
    ImageRenderable(unsigned char r, unsigned char g, unsigned char b, unsigned char a): r_(r), g_(g), b_(b), a_(a) {}
    virtual ~ImageRenderable() = default;

    virtual void render(int max_width, int max_height, png_bytep* &png_vec) = 0;

    void setPoint(int x, int y);
    R2Point& getPoint();
    bool getSkipRender();
    void setSkipRender(bool skip_render);

    void setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};



#endif //RENDERABLE_H
