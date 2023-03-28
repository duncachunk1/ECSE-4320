#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>

#include "hashtable.h"
#include "encoder.h"

using namespace std::chrono;

int lineNum = 0;
std::vector<int> vanillaWord;

std::pair<std::string, int> processLine(std::ifstream &infile, int thread_id) {
    std::string line;
    std::getline(infile, line);
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    int r = ++lineNum;
    return std::make_pair(line, r);
}

//dumb way to look for a string in a file
void vanillaSearch(std::ifstream &infile, std::string& word, int thread_id) {
	auto temp = processLine(infile, thread_id);
	if (!temp.first.empty() && temp.first == word) {
		std::cout << temp.second << std::endl;
		vanillaWord.push_back(temp.second);
	}
}


//function to ease encoding with parallel programming
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

    auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

    //read data into the program depending on the desired number of threads
    std::thread threads[numThreads];
    Encoder<std::string> encoders[numThreads];
    bool flag = false;
    start = high_resolution_clock::now();
    while (!infile.eof()) {
	    for (int i = 0; i < numThreads; ++i) {
	        threads[i] = std::thread(encodingNeeds<std::string>, std::ref(encoders[i]), std::ref(infile), i, std::ref(ht));
	    }
	    for (int i = 0; i < numThreads; ++i) {
	        threads[i].join();
	    }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    std::cout << "Encoding Time: " << duration.count() << " ms" << std::endl;


    std::vector<std::pair<int, std::string> > vanillaPrefix;
    std::vector<int> vanillaWord;

    std::string input;
    std::string pref;
    std::string searchType;

    
    
    std::cout << "Enter your input or hit '$' to quit: ";
	std::cin >> input;
    while (input != "$") {
    	std::cout << "Would you rather the 'baseline' search or 'encoded' search: " << std::endl;
    	std::cin >> searchType;

    	if (searchType == "baseline") {
    		
    		//resets to beginning of file
    		infile.close();
    		std::ifstream infile(argv[1]);

    		lineNum = 0;
    		input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
    		input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
    		
    		start = high_resolution_clock::now();
    		while (!infile.eof()) {
			    for (int i = 0; i < numThreads; ++i) {
			        threads[i] = std::thread(vanillaSearch, std::ref(infile), std::ref(input), i);
			    }
			    for (int i = 0; i < numThreads; ++i) {
			        threads[i].join();
			    }
		    }
		    stop = high_resolution_clock::now();
		    std::cout << "Key: " << input << std::endl;
    		std::cout << "Occurrences: " << vanillaWord.size() << std::endl;
    		for (int i = 0; i < vanillaWord.size(); i++) {
    			std::cout << "\t" << vanillaWord[i] << std::endl;
    		}
    		duration = duration_cast<milliseconds>(stop - start);
    		std::cout << "Duration: " << duration.count() << " ms" << std::endl;
		    vanillaWord.clear();
    	}
    	else if (searchType == "encoded") {
    		input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
		    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
		    encoders[0].setData(input);
		    encoders[0].encodeData();
		    input = encoders[0].getEncodedData();
		    start = high_resolution_clock::now();
		    print_search(ht, input);
		  	stop = high_resolution_clock::now();
		    duration = duration_cast<milliseconds>(stop - start);
		    std::cout << "Duration: " << duration.count() << " ms" << std::endl;
    	}
	  	std::cout << "Enter your input or hit 'enter' to quit: ";
	  	std::cin >> input;
    }
    

    infile.close();
    free_table(ht);

    return 0;
}