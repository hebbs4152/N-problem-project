#ifndef OTHER_H
#define OTHER_H


#include "Body.h"
#include "QuadTree.h"
#include <math.h>


void moveBody(Body & b);



//Calculate and update force on body lhs
void calculateForce(Body & lhs, const Body & rhs);


//Generate random double
double fRand(double fMin, double fMax);


#endif