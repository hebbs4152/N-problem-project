#ifndef QUADTREE_H
#define QUADTREE_H

#include "Body.h"
#include <vector>

using namespace std;

class QuadTree {
private:
    Body avgBody = Body();
    int bodiesInFrame = 0;
    const int capacity = 1;
    QuadTree* children[4] = { nullptr };
    vector<Body> bodies; //Vector size is maximum 1


public:
    double x, y, width, height = 0;

    double far = 0;

    QuadTree(double x, double y, double width, double height) : x(x), y(y), width(width), height(height) {}

    QuadTree(double x, double y, double width, double height, double far) : x(x), y(y), width(width), height(height), far(far) {}

    ~QuadTree();

    bool isLeaf();

    void subdivide();

    void updateAverage(Body body);

    bool foundQuadrant = false;

    bool insert(Body& body);

    bool contains(Body body);


    double quotient(const QuadTree & internalNode, const Body & b);

    //Updates the force on body b in current Quadrant
    void updateForce(Body & b);



};

void moveBody(Body & b);

ostream& operator<<(ostream& os, const QuadTree& f);


#endif