//
// Created by HyunJin SO on 2025. 11. 30..
//

#ifndef BLOCK_H
#define BLOCK_H

#include "Ball.h"
#include "ImageRenderable.h"


class Block : public ImageRenderable {
private:
    int width = 0;
    int height = 0;
    int hp = 0;
    int maxHp = 0;
public:
    Block(int x, int y, int width, int height, int maxHp, int hp,
        unsigned char R, unsigned char G, unsigned char B);

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
