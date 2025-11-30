//
// Created by HyunJin SO on 2025. 11. 27..
//

#ifndef ARROWHELPER_H
#define ARROWHELPER_H
#include "ImageRenderable.h"


class ArrowHelper : public ImageRenderable {
    double radius, theta;
public:
    ArrowHelper(int center_x, int center_y, double radius, double theta,
        unsigned char R, unsigned char G, unsigned char B, unsigned char A);

    void render(int max_width, int max_height, png_bytep* &png_vec) override;

    void setRadius(double radius);
    void setTheta(double theta);
};



#endif //ARROWHELPER_H
