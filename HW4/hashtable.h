#ifndef hashtable_h
#define hashtable_h

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <zlib.h>

template<typename KeyType, typename ValueType>
class HashTable {
public:
    HashTable();
    HashTable(size_t capacity);

    void insert(const KeyType& key, const ValueType& value);
    bool contains(const KeyType& key) const;
    ValueType& operator[](const KeyType& key);
    const ValueType& operator[](const KeyType& key) const;
    void remove(const KeyType& key);
    size_t size() const;
    bool empty() const;
    void clear();
    

    

private:
    // Hash table implementation details go here
    std::compressor a;

};


#endif