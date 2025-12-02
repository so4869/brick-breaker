//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef R2POINT_H
#define R2POINT_H



class R2Point {
protected:
    double x_ = 0, y_ = 0;

public:
    double get_x();
    double get_y();

    void setX(double x);
    void setY(double y);
    void setXY(double x, double y);
};



#endif //R2POINT_H
