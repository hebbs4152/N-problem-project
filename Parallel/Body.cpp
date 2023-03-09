#include "Body.h"
#include <cmath>

#define EPSILON 0.1

using namespace std;




    std::ostream& operator<<(std::ostream& os, const Body& b) {
        os << "Body: [x=" << b.x << ", y=" << b.y << "], [v_x=" << b.v_x << ", v_y=" << b.v_y << 
        "], [f_x=" << b.f_x << ", f_y=" << b.f_y << "], mass=" << b.mass << endl; 
        return os;
    }

    Body & Body::operator+(const Body& b) {
        this->x = this->x + b.x;
        this->y = this->y + b.y;
        this->v_x = this->v_x + b.v_x;
        this->v_y = this->v_y + b.v_y;
        this->f_x = this->f_x + b.f_x;
        this->f_y = this->f_y + b.f_y;
        this->mass = this->mass + b.mass;

        return *this;
    }

    Body & Body::operator/(const double& b) {
        this->x = this->x / b;
        this->y = this->y / b;
        this->v_x = this->v_x / b;
        this->v_y = this->v_y / b;
        this->f_x = this->f_x / b;
        this->f_y = this->f_y / b;
        this->mass = this->mass / b;

        return *this;
    }

    Body & Body::operator*(const Body& b) {
        this->x = this->x * b.x;
        this->y = this->y * b.y;
        this->v_x = this->v_x * b.v_x;
        this->v_y = this->v_y * b.v_y;
        this->f_x = this->f_x * b.f_x;
        this->f_y = this->f_y * b.f_y;
        this->mass = this->mass * b.mass;

        return *this;
    }

    Body & Body::operator*(const double& b) {
        this->x = this->x * b;
        this->y = this->y * b;
        this->v_x = this->v_x * b;
        this->v_y = this->v_y * b;
        this->f_x = this->f_x * b;
        this->f_y = this->f_y * b;
        this->mass = this->mass * b;

        return *this;
    }

    bool Body::operator==(const Body& b) {
        return fabs(this->x - b.x) < EPSILON && fabs(this->y - b.y) < EPSILON;
    }
