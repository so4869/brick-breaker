//
// Created by HyunJin SO on 2025. 11. 30..
//

#ifndef BLOCK_H
#define BLOCK_H

#include "Ball.h"
#include "ImageRenderable.h"


class Block : public ImageRenderable {
private:
    int width_ = 0;
    int height_ = 0;
    int hp_ = 0;
    int max_hp_ = 0;
public:
    Block(int x, int y, int width, int height, int max_hp, int hp,
        unsigned char r, unsigned char g, unsigned char b);

    int getWidth();
    void setWidth(int width);
    int getHeight();
    void setHeight(int height);

    int getHp();
    void setHp(int hp);

    int meet(Ball &ball);
    void hit(Ball &ball);

    void render(int max_width, int max_height, png_bytep* &png_vec) override;
};



#endif //BLOCK_H
