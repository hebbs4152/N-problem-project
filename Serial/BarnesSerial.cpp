#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <fstream>
#include "gnuplot-iostream.h" // include gnuplot header
#include <boost/tuple/tuple.hpp>
#include <chrono>
#include <thread>

using namespace std;

//Low far, low amount of approximations
double far = 0.0001;
#define DT 0.01
#define G 6.67e-11
// #define G 6.67e-2
#define SIZE 9e7
#define TIMELIMIT 100
#define EPSILON 0.1

class Body {
public:
    double x, y = 0;

    double v_x, v_y = 0;

    double f_x, f_y = 0;

    double mass = 0;


    Body() {
        this->x = 0;
        this->y = 0;
        this->mass = 0;
    }

    Body(double x, double y, double mass) {
        this->x = x;
        this->y = y;
        this->mass = mass;
    }

    Body(double x, double y, double v_x, double v_y, double f_x, double f_y, double mass) {
        this->x = x;
        this->y = y;
        this->v_x = v_x;
        this->v_y = v_y;
        this->f_x = f_x;
        this->f_y = f_y;
        this->mass = mass;
    }

    friend ostream& operator<<(ostream& os, const Body& b) {
        os << "Body: [x=" << b.x << ", y=" << b.y << "], [v_x=" << b.v_x << ", v_y=" << b.v_y << 
        "], [f_x=" << b.f_x << ", f_y=" << b.f_y << "], mass=" << b.mass << endl; 
        return os;
    }

    Body & operator+(const Body& b) {
        this->x = this->x + b.x;
        this->y = this->y + b.y;
        this->v_x = this->v_x + b.v_x;
        this->v_y = this->v_y + b.v_y;
        this->f_x = this->f_x + b.f_x;
        this->f_y = this->f_y + b.f_y;
        this->mass = this->mass + b.mass;

        return *this;
    }

    Body & operator/(const double& b) {
        this->x = this->x / b;
        this->y = this->y / b;
        this->v_x = this->v_x / b;
        this->v_y = this->v_y / b;
        this->f_x = this->f_x / b;
        this->f_y = this->f_y / b;
        this->mass = this->mass / b;

        return *this;
    }

    Body & operator*(const Body& b) {
        this->x = this->x * b.x;
        this->y = this->y * b.y;
        this->v_x = this->v_x * b.v_x;
        this->v_y = this->v_y * b.v_y;
        this->f_x = this->f_x * b.f_x;
        this->f_y = this->f_y * b.f_y;
        this->mass = this->mass * b.mass;

        return *this;
    }

    Body & operator*(const double& b) {
        this->x = this->x * b;
        this->y = this->y * b;
        this->v_x = this->v_x * b;
        this->v_y = this->v_y * b;
        this->f_x = this->f_x * b;
        this->f_y = this->f_y * b;
        this->mass = this->mass * b;

        return *this;
    }

    bool operator==(const Body& b) {
        return fabs(this->x - b.x) < EPSILON && fabs(this->y - b.y) < EPSILON;
    }
};

//Calculate and update force on body lhs
void calculateForce(Body & lhs, const Body & rhs) {
    double distance = sqrt( pow(lhs.x - rhs.x,2) + pow(lhs.y - rhs.y,2));
    double magnitude = (G*lhs.mass*rhs.mass) / pow(distance,2);
    vector<double> direction = {rhs.x - lhs.x, rhs.y - lhs.y};
    lhs.f_x += magnitude*direction[0]/distance;
    lhs.f_y += magnitude*direction[1]/distance;
}


class QuadTree {
private:
    // double avg_x, avg_y = 0;
    // double avg_v_x, avg_v_y = 0;
    // double avg_f_x, avg_f_y = 0;
    // double avg_mass = 0;
    Body avgBody = Body();
    int bodiesInFrame = 0;
    const int capacity = 1;
    QuadTree* children[4] = { nullptr };
    vector<Body> bodies;
    double x, y, width, height = 0;

public:
    friend ostream& operator<<(ostream& os, const QuadTree& f) {
        os << "Quadrant: " << endl;
        os << f.x << ", " << f.y+f.height << "    " << f.x+f.width << ", " <<f.y+f.height << endl;
        os << f.x << ", " << f.y << "    " << f.x+f.width << ", " <<f.y << endl;
        return os;
    }

    QuadTree(double x, double y, double width, double height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    bool isLeaf() {
        return children[0] == nullptr;
    }

    void subdivide() {
        double halfWidth = width / 2;
        double halfHeight = height / 2;
        children[0] = new QuadTree(x, y, halfWidth, halfHeight);
        children[1] = new QuadTree(x + halfWidth, y, halfWidth, halfHeight);
        children[2] = new QuadTree(x, y + halfHeight, halfWidth, halfHeight);
        children[3] = new QuadTree(x + halfWidth, y + halfHeight, halfWidth, halfHeight);
        
        // Move the current body in this quadrant into on of its children
        if (bodies.size() == 1) {
            Body a = bodies[0];
            bodies.pop_back();
            for (int i = 0; i < 4; i++) {
                if (children[i]->contains(a)) {
                    children[i]->insert(a);
                    break; //Shouldn't matter
                }
            }
        }
    }

    void updateAverage(Body body) {
        avgBody = avgBody * bodiesInFrame;
        avgBody = avgBody + body;
        bodiesInFrame++;
        avgBody = avgBody / bodiesInFrame;



        // avg_x *= bodiesInFrame;
        // avg_y *= bodiesInFrame;
        // avg_v_x *= bodiesInFrame;
        // avg_v_y *= bodiesInFrame;
        // avg_f_x *= bodiesInFrame;
        // avg_mass *= bodiesInFrame;
        
        // avg_x += body.x;
        // avg_y += body.y;
        // avg_v_x += body.v_x;
        // avg_v_y += body.v_y;
        // avg_f_x += body.f_x;
        // avg_f_y += body.f_y;
        // avg_mass += body.mass;

        // bodiesInFrame++;
        // avg_x /= bodiesInFrame;
        // avg_y /= bodiesInFrame;
        // avg_v_x /= bodiesInFrame;
        // avg_v_y /= bodiesInFrame;
        // avg_f_x /= bodiesInFrame;
        // avg_mass /= bodiesInFrame;
    }

    bool insert(Body& body) {
        if (!contains(body)) {
            return false;
        }

        //This body will be inserted somewhere into this quadrant, update the quadrants averages

        updateAverage(body);

        if (bodies.size() < capacity && isLeaf()) {
            bodies.push_back(body);
            return true;
        }

        if (isLeaf()) {
            subdivide();
        }

        for (int i = 0; i < 4; i++) {
            if (children[i]->insert(body)) {
                return true;
            }
        }

        return false;
    }

    bool contains(Body body) {
        return body.x >= x && body.x <= x + width &&
               body.y >= y && body.y <= y + height;
    }


    double quotient(const QuadTree & internalNode, const Body & b) {
        double s = internalNode.width;
        double d = sqrt( pow(b.x - internalNode.avgBody.x,2) + pow(b.y - internalNode.avgBody.y,2));
        return s/d;
    }

    // double quotient(const QuadTree & internalNode, const Body & b) {
    //     double d = sqrt( pow(b.x - internalNode.avg_x,2) + pow(b.y - internalNode.avg_y,2));
    //     return d;
    // }

    //Updates the force on body b in current Quadrant
    void updateForce(Body & b) {
        if (isLeaf() && bodies.size() == 0) {
            //empty quadrant
            return;
        }


        double q = quotient(*this, b);
        // cout << "hi" << endl;

        
        if (this->isLeaf() && this->bodies.size() == 1) {
            // cout << 1 << endl;
            // cout << this->bodies[0] << endl;
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

};

//Generate random double
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char** argv) {

    if (argc != 4 && argc != 5) {
        cout << "Incorrect amount of arguments!" << endl;
        return 0;
    }
    auto start = std::clock();
    // Random seed
    srand(time(NULL));


    int gnumBodies = atoi(argv[1]);
    int numSteps = atoi(argv[2]);
    far = atof(argv[3]);

    string gui = "";

    if (argc == 5)
        gui = argv[4];

    bool guiEnable = false;

    if ( gui == "-gui")
        guiEnable = true;

    Gnuplot gp; // create a gnuplot object 
    if (guiEnable) {
        

        gp << "set xrange [0:" << SIZE << "]\n";
        gp << "set yrange [0:" << SIZE << "]\n";
    }




    cout << "gnumBodies: " << gnumBodies << endl;
    cout << "numSteps: " << numSteps << endl;
    cout << "far: " << far << endl;
    
    vector<Body> bodies(gnumBodies);
    for (int i = 0; i < gnumBodies; i++) {
        bodies[i] = Body(fRand(0, SIZE), fRand(0, SIZE), fRand(6e22, 2e30)); //, fRand(-5, 5), fRand(-5,5));
    }

    srand (time(NULL));

    for (int i = 0; i < numSteps; i++) {
        QuadTree tree(0, 0, SIZE, SIZE);
        for (int j = 0; j < gnumBodies; j++) {
            tree.insert(bodies[j]);
        }

        for (int j = 0; j < gnumBodies; j++) {
            tree.updateForce(bodies[j]);
        }

        for (int j = 0; j < gnumBodies; j++) {
            tree.moveBody(bodies[j]);
            // if (j == 0)
            //     cout << bodies[j] << endl;
        }

        // Plot the bodies using gnuplot
        if (guiEnable) {
            ofstream tmpfile("data.tmp");
            for (int j = 0; j < gnumBodies; j++) {
                tmpfile << bodies[j].x << " " << bodies[j].y << endl;
            }
            tmpfile.close();

            gp << "plot 'data.tmp' with points title 'Step " << i << "'\n";

            gp<<"pause 0.1\n"; //pause for 0.1 seconds before continuing
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            gp.flush();
        }
        
    }

    auto end = time(NULL);

    cout << "Time taken was " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms"<< endl;

}