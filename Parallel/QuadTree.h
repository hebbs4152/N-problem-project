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
    double x, y, width, height = 0;

public:
    

    double far = 0;

    QuadTree(double x, double y, double width, double height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    QuadTree(double x, double y, double width, double height, double far) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->far = far;
    }

    ~QuadTree() {
        if (!isLeaf()) {
            for (int i = 0; i < 4; i++) {
                delete children[i];
            }
        }
    }

    bool isLeaf();

    void subdivide();

    void updateAverage(const Body& body);

    bool foundQuadrant = false;

    bool insert(Body& body);

    bool contains(Body body);


    double quotient(const QuadTree & internalNode, const Body & b);

    //Updates the force on body b in current Quadrant
    void updateForce(Body & b);



};

ostream& operator<<(ostream& os, const QuadTree& f);


#endif