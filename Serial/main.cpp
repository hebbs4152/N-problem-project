#include "Other.h"
#include "Body.h"
#include "QuadTree.h"
#include "gnuplot-iostream.h" // include gnuplot header
#include <chrono>
#include <thread>



#define SIZE 9e7

double far;

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

    for (int i = 0; i < numSteps; i++) {
        QuadTree tree(0, 0, SIZE, SIZE, far);
        for (int j = 0; j < gnumBodies; j++) {
            tree.insert(bodies[j]);
        }

        for (int j = 0; j < gnumBodies; j++) {
            tree.updateForce(bodies[j]);
        }

        for (int j = 0; j < gnumBodies; j++) {
            moveBody(bodies[j]);
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