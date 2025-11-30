//
// Created by HyunJin SO on 2025. 11. 25..
//

#include "R2Point.h"

double R2Point::get_x() {
    return this->x;
}

double R2Point::get_y() {
    return this->y;
}


void R2Point::set_x(double x){
    this->x = x;
}

void R2Point::set_y(double y){
    this->y = y;
}

void R2Point::set_xy(double x, double y){
    this->x = x;
    this->y = y;
}
