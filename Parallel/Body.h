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

    Body(double x_, double y_, double v_x_, double v_y_, double f_x_, double f_y_, double mass_) : x(x_), y(y_), v_x(v_x_), v_y(v_y_),
                                                                                            f_x(f_x_), f_y(f_y_), mass(mass_) {}



    Body & operator+(const Body& b);

    Body & operator/(const double& b);

    Body & operator*(const Body& b);

    Body & operator*(const double& b);

    bool operator==(const Body& b);
};

    ostream& operator<<(ostream& os, const Body& b);

#endif