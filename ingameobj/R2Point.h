//
// Created by HyunJin SO on 2025. 11. 25..
//

#ifndef R2POINT_H
#define R2POINT_H



class R2Point {
protected:
    int x = 0, y = 0;

public:
    int get_x();
    int get_y();

    void set_x(int x);
    void set_y(int y);
    void set_xy(int x, int y);
};



#endif //R2POINT_H
