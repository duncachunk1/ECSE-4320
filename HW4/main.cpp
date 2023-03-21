#include <iostream>
#include <thread>
#include <fstream>

#include "hashtable.h"
#include "encoder.h"


//temporary function Im playing around with to try and solve the issue with parallel programming. Something
//about a static function
template <typename dataType>
void encodingNeeds(Encoder<dataType>& e, const dataType d) {
	e.setData(d);
	e.encodeData();
}

int main(int argc, char **argv) {

	//arg1 will be the input file
	//arg2 will be the num of threads

	if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input_file" << std::endl;
        return 1;
    }

    //checks if file can be opened
    std::string filename = argv[1];
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    //thread count
    const int numThreads = *argv[2];
    if (numThreads == 0) {
    	std::cerr << "You must input a number of threads, the current threaad count is 0" << std::endl;
    }

    //read data into the program depending on the desired number of threads
    std::string data[numThreads];
    std::string dataRead;    

    std::thread threads[numThreads];
    Encoder<std::string> encoders[numThreads];

    //set up the threads to encode the data in parallel
    //im having trouble encoding in parallel, probably a stupid fix
    //currently this reads in each line of th einput file,
    //assigns that data to an encoder
    //and eventually each encode will encode that data in parallel.
    //Once we can encode in parallel, it will make sense to set the data in parallel to
    for (int i = 0; i < numThreads; i++) {
    	std::getline(infile, data[i]);
    	encoders[i] = Encoder<std::string>();
    	encoders[i].setData(data[i]);
    	//threads[i] = std::thread(encodingNeeds<std::string>, std::ref(encoders[i]));
    }

    //insert encoded items into the hash table, implementing that right now

    return 0;
}