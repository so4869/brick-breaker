//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <pngconf.h>
#include <vector>

#include "R2Point.h"


class ImageRenderable {
protected:
    R2Point point;
    unsigned char R = 0, G = 0, B = 0, A = 0;
    bool skipRender = false;
public:
    ImageRenderable() = default;
    ImageRenderable(unsigned char R, unsigned char G, unsigned char B, unsigned char A): R(R), G(G), B(B), A(A) {}
    virtual ~ImageRenderable() = default;

    virtual void render(int max_width, int max_height, png_bytep* &png_vec) = 0;

    void setR2Point(int x, int y);
    bool getSkipRender();
    void setSkipRender(bool skipRender);
};



#endif //RENDERABLE_H
