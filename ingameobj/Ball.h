//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef BALL_H
#define BALL_H
#include "ImageRenderable.h"

void drawFilledCircleRGBA(
    png_bytep* img,
    int width, int height,
    int cx, int cy, int r,
    unsigned char R,
    unsigned char G,
    unsigned char B,
    unsigned char A);


class Ball : public ImageRenderable {
private:
    double radius_ = 0.0;
    int num_ = 1;
    double dx_ = 0, dy_ = 0;
public:
    Ball(int center_x, int center_y, double radius,
        unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    void render(int max_width, int max_height, png_bytep* &png_vec) override;

    double getRadius();

    double getDx();
    void setDx(double dx);
    double getDy();
    void setDy(double dy);
    void move();

    void reverseDx();
    void reverseDy();

    void setNum(int num);
};



#endif //BALL_H
