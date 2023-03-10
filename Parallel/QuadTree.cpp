#include "QuadTree.h"
#include "Body.h"
#include "Other.h"
#include <omp.h>
#include <stdlib.h>     /* srand, rand */
#include <stdio.h> 
#include <math.h>
#include <cmath>
#include <vector>
#include <thread>

using namespace std;

    // ostream& operator<<(ostream& os, const QuadTree& f) {
    //     os << "Quadrant: " << endl;
    //     os << f.x << ", " << f.y+f.height << "    " << f.x+f.width << ", " <<f.y+f.height << endl;
    //     os << f.x << ", " << f.y << "    " << f.x+f.width << ", " <<f.y << endl;
    //     return os;
    // }

    bool QuadTree::isLeaf() {
        return children[0] == nullptr;
    }

    void QuadTree::subdivide() {
        double halfWidth = width / 2;
        double halfHeight = height / 2;
        children[0] = new QuadTree(x, y, halfWidth, halfHeight);
        children[1] = new QuadTree(x + halfWidth, y, halfWidth, halfHeight);
        children[2] = new QuadTree(x, y + halfHeight, halfWidth, halfHeight);
        children[3] = new QuadTree(x + halfWidth, y + halfHeight, halfWidth, halfHeight);
        
    }

    void QuadTree::updateAverage(const Body& body) {
        avgBody = avgBody * bodiesInFrame;
        avgBody = avgBody + body;
        bodiesInFrame++;
        avgBody = avgBody / bodiesInFrame;
    }

    bool QuadTree::insert(Body& body) {
        if (foundQuadrant == true)
            return false;

        if (!contains(body)) {
            return false;
        }

        //This body will be inserted somewhere into this quadrant, update the quadrants averages
        #pragma omp critical
        updateAverage(body);

        bool pushedBack = false;
        if (bodies.size() < capacity && isLeaf()) {
            #pragma omp critical 
            {
            if (bodies.size() < capacity && isLeaf()) {
                bodies.push_back(body);
                pushedBack = true;
            }
            }
        }
        if (pushedBack)
            return true;

        bool createdChildren = false;
        //Double checking lock
        if (isLeaf()) {
            #pragma omp critical
            if (isLeaf()) {
                subdivide();
                createdChildren = true;
                
            }
        }
// Move the current body in this quadrant into on of its children
        if (createdChildren) {
            Body a = bodies[0];
            bodies.pop_back();
            for (int i = 0; i < 4; i++) {
                if (children[i]->contains(a)) {
                    children[i]->insert(a);
                    break; //Shouldn't matter
                }
            }
        }


        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            if (children[i]->insert(body)) {
                foundQuadrant = true;
            }
        }
        if (foundQuadrant) {
            foundQuadrant = false;
            return true;
        }
        

        return false;
    }

    bool QuadTree::contains(Body body) {
        return body.x >= x && body.x <= x + width &&
               body.y >= y && body.y <= y + height;
    }


    double QuadTree::quotient(const QuadTree & internalNode, const Body & b) {
        double s = internalNode.width;
        double d = sqrt( pow(b.x - internalNode.avgBody.x,2) + pow(b.y - internalNode.avgBody.y,2));
        return s/d;
    }


    //Updates the force on body b in current Quadrant
    void QuadTree::updateForce(Body & b) {
        if (isLeaf() && bodies.size() == 0) {
            //empty quadrant
            return;
        }

        double q = quotient(*this, b);
        // cout << "hi" << endl;

        
        if (this->isLeaf() && this->bodies.size() == 1) {

            if (b == this->bodies[0]) {
                return;
            }
            calculateForce(b, this->bodies[0]);
        }

        //This is not a leaf, if the quotient is less than the far, treat the internal node as a single node
        else if (q < far) {
            // cout << "APPROXIMATION ONLINE" << endl;
            // Body otherBody(this->avg_x, this->avg_y, this-> avg_v_x, this->avg_v_y, this->avg_f_x,
            //                 this->avg_f_y,this->avg_mass);
            if (b == avgBody) {
                return;
            }
            calculateForce(b, avgBody);
        }
        // If this quadrant isn't a leaf/external node and q is bigger than far, then recursively visit the children
        else if(! this->isLeaf()) {
            // cout << 3 << endl;
            for (int i = 0; i < 4; i++) {
                this->children[i]->updateForce(b);
            }
        }
        else {
            // cout << "Visited a quadrant without bodies" << endl;
        }
    }
