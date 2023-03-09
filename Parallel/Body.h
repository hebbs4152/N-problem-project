#ifndef BODY_H
#define BODY_H

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>  

using namespace std;

class Body {
public:
    double x, y = 0;

    double v_x, v_y = 0;

    double f_x, f_y = 0;

    double mass = 0;


    Body() : x(0), y(0), mass(0) {}

    Body(double x_, double y_, double mass_)  : x(x_), y(y_), mass(mass_) {}

    Body(double x, double y, double v_x, double v_y, double f_x, double f_y, double mass) : x(x), y(y), v_x(v_x), v_y(v_y),
                                                                                            f_x(f_x), f_y(f_y), mass(mass) {}



    Body & operator+(const Body& b);

    Body & operator/(const double& b);

    Body & operator*(const Body& b);

    Body & operator*(const double& b);

    bool operator==(const Body& b);
};

    ostream& operator<<(ostream& os, const Body& b);

#endif