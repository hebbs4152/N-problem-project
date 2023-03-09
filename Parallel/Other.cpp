#include "Other.h"
#include "Body.h"

#define G 6.67e-11
#define DT 0.01

//Calculate and update force on body lhs
void calculateForce(Body & lhs, const Body & rhs) {
    double distance = sqrt( pow(lhs.x - rhs.x,2) + pow(lhs.y - rhs.y,2));
    double magnitude = (G*lhs.mass*rhs.mass) / pow(distance,2);
    vector<double> direction = {rhs.x - lhs.x, rhs.y - lhs.y};
    lhs.f_x += magnitude*direction[0]/distance;
    lhs.f_y += magnitude*direction[1]/distance;
}


void moveBody(Body & b) {
    double delta_v_x = b.f_x/b.mass * DT;
    double delta_v_y = b.f_y/b.mass * DT;
    double delta_p_x = (b.v_x + delta_v_x/2) * DT;
    double delta_p_y = (b.v_y + delta_v_y/2) * DT;
    b.v_x += delta_v_x;
    b.v_y += delta_v_y;
    // cout << delta_p_x << endl;
    b.x += delta_p_x;
    b.y += delta_p_y;
    b.f_x = 0;
    b.f_y = 0;
}

//Generate random double
double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}