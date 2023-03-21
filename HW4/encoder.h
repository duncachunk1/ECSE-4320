#ifndef encoder_h
#define encoder_h

/*

This templated encoder class utilizes Run Length encoding. This essentially encodes a sequence of letters.
For example:
    AAABBCCCDEE --> 3A2B3C1D2E

RLE is a generally useful encoding algorithm on data with low cardinality. 

The template/typename heywords are implemented so that this class can encode a data input file with any kind of input data type

*/

#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <sstream>

template<typename dataType>
class Encoder {
public:

    //CONSTRUCTORS
    Encoder();

    //GETTERS
    dataType getData() const;
    std::string getEncodedData() const;

    //SETTERS
    void setData(const dataType& d);
    void encodeData();

    //MIISCELLANEOUS
    void print(std::ostream& os);

private:
    std::string encodedData;
    dataType originalData;
    //static std::unordered_map<std::string, dataType> dict;
};

template<typename dataType>
Encoder<dataType>::Encoder() {
    //std::cout << "Encoder initialized" << std::endl;
    //originalData = 0;   //arbitrary initial value
}

template<typename dataType>
dataType Encoder<dataType>::getData() const {
    return originalData;
}

template<typename dataType>
std::string Encoder<dataType>::getEncodedData() const {

    if (encodedData.empty()) {
        std::cerr << "You must encode the data before you can retreive the encoded data" << std::endl;
    }
    return encodedData;
}

template<typename dataType>
void Encoder<dataType>::setData(const dataType& d) {
    originalData = d;
}

template<typename dataType>
void Encoder<dataType>::encodeData() {
    int counter = 1;
    std::string castedData;

    //*
    if (std::is_same<dataType, std::string>::value) {
        castedData = originalData;
    } else {
        std::ostringstream oss;
        oss << originalData;
        castedData = oss.str();
    }
    
    

    for(int i = 0; i < castedData.size(); i++) {
        if (castedData[i] == castedData[i+1]) {
            counter++;
        }
        else {
            encodedData += std::to_string(counter);
            encodedData += castedData[i];
            counter = 1;
        }
    }
    /*
    auto it = dict.find(encodedData);
    if (it == dict.end()) {
        dict[encodeData] = originalData; // add new item to dictionary
    }
    */
}

template<typename dataType>
void Encoder<dataType>::print(std::ostream& os) {
    os << "Original Data: " << originalData << std::endl;
    os << "Encoded Data: " << encodedData << std::endl;
}


#endif