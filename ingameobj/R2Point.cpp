//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "R2Point.h"

int R2Point::get_x() {
    return this->x;
}

int R2Point::get_y() {
    return this->y;
}


void R2Point::set_x(int x){
    this->x = x;
}

void R2Point::set_y(int y){
    this->y = y;
}

void R2Point::set_xy(int x, int y){
    this->x = x;
    this->y = y;
}
