//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef R2POINT_H
#define R2POINT_H



class R2Point {
protected:
    double x = 0, y = 0;

public:
    double get_x();
    double get_y();

    void set_x(double x);
    void set_y(double y);
    void set_xy(double x, double y);
};



#endif //R2POINT_H
