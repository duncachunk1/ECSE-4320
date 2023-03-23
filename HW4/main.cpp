#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "hashtable.h"
#include "encoder.h"

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
int lineNum = 0;


std::pair<std::string, int> processLine(std::ifstream &infile, int thread_id) {
    std::string line;
    std::getline(infile, line);
    int r = ++lineNum;
    return std::make_pair(line, r);
}
//temporary function Im playing around with to try and solve the issue with parallel programming. Something
//about a static function
template <typename dataType>
void encodingNeeds(Encoder<dataType>& e, std::ifstream &infile, int thread_id, HashTable* h) {
	std::pair<std::string, int> p = processLine(infile, thread_id);
	std::string d = p.first;
	int r = p.second;
	if (!d.empty()){
		e.setData(d);
	    e.encodeData();
	    //e.print(std::cout);
	    ht_insert(h, e.getEncodedData(), r);
	    
	}
}

int main(int argc, char **argv) {

	//arg1 will be the input file
	//arg2 will be the num of threads

	if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " input_file num_threads" << std::endl;
        std::cerr << "ARGC: " << argc << std::endl;
        std::cerr << argv[0] << ", " << argv[1] << ", " << argv[2] << std::endl;
        return 1;
    }

    //create table object
    HashTable *ht = create_table(CAPACITY);

    //checks if file can be opened
    std::string filename = argv[1];
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }
    if (infile.peek() == std::ifstream::traits_type::eof()) {
	    std::cerr << "Error: Input file is empty" << std::endl;
	    return 1;
	}

    //thread count
    const int numThreads = std::stoi(argv[2]);
    if (numThreads == 0) {
    	std::cerr << "You must input a number of threads, the current threaad count is 0" << std::endl;
    }

    std::cout << "Processing the data..." << std::endl;

    //read data into the program depending on the desired number of threads
    std::thread threads[numThreads];
    Encoder<std::string> encoders[numThreads];
    bool flag = false;
    while (!infile.eof()) {
	    for (int i = 0; i < numThreads; ++i) {
	        threads[i] = std::thread(encodingNeeds<std::string>, std::ref(encoders[i]), std::ref(infile), i, std::ref(ht));
	    }
	    for (int i = 0; i < numThreads; ++i) {
	        threads[i].join();
	    }
    }

	//std::cout << "Hello" << std::endl;
    bool ena;
    std::string input;
    std::cout << "Type (yes/no) if you want to enable SIMD commands";
	std::cin >> input;
    if (input == "yes"){
        std::cerr <<"SIMD on"<<std::endl;
        ena = true;
    }
    else{
        std::cerr <<"SIMD off"<<std::endl;
        ena = false;
    }
    //*
    std::cout << "Enter your input or hit '$' to quit: ";
	std::cin >> input;
    while (input != "$") {
	  	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
	    encoders[0].setData(input);
	    encoders[0].encodeData();
	    input = encoders[0].getEncodedData();
	    print_search(ht, input, ena);
	    //print_table(ht);
	    std::cout << "Enter your input or hit 'enter' to quit: ";
	  	std::cin >> input;
    }
    //*/
    
    //print_table(ht);

    free_table(ht);
	
	infile.close();
    return 0;
}