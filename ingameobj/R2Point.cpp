//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "R2Point.h"

double R2Point::get_x() {
    return this->x_;
}

double R2Point::get_y() {
    return this->y_;
}


void R2Point::setX(double x){
    this->x_ = x;
}

void R2Point::setY(double y){
    this->y_ = y;
}

void R2Point::setXY(double x, double y){
    this->x_ = x;
    this->y_ = y;
}
