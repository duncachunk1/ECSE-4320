#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

/*  
*   This file is for the sole purpose of creating text files of different sizes
*   containing integers to fill a b-tree. The first argument is the name of the
*   file and the next argument is how many values you want present in the file.
*/

int main(int argc, char **argv) {
    std::ofstream outfile(argv[1]);
    if (!outfile.is_open()) {
        std::cerr << "Error: could not open file" << std::endl;
        return 1;
    }

    // Generate random number
    srand(time(0));
    int randInt;
    unsigned int size = std::stoi(argv[2]);
    for (unsigned int i = 0; i < size; i++) {
        randInt = rand() % 20;
        outfile << randInt << std::endl;
    }
    

    outfile.close();
    return 0;
}
