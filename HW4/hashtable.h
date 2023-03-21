#ifndef hashtable_h
#define hashtable_h

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <list>


//template and typename are generics so it could be a hash table of any type
template<typename KeyType, typename ValueType>
class HashTable {
public:
    
    //CONSTRUCTORS
    HashTable();
    HashTable(size_t capacity);


    //GETTERS
    bool contains(const KeyType& key) const; //checks to see if a particular key is present
    bool contains(const ValueType& value) const; //checks to tsee if a particular value is present
    ValueType query(const KeyType& key) const;  //returns the specified value from the table
    size_t size() const;    //size of the table
    bool isEmpty() const;   //checks if the table/key contains any elements

    //SETTERS
    void insert(const KeyType& key, const ValueType& value);    //inserts a value into a given key index
    void remove(const KeyType& key);    //removes a value
    void clear();   //wipes the whole table

    //OPERATOR OVERLOADS
    ValueType& operator[](const KeyType& key);  //operator overload, not sure if im gonna implement this

    //MISCELLANEOUS
    KeyType hashFunction(ValueType& val); //hash function   

private:
    // Hash table implementation details go here
    static const int hashGroups;    //number of indices in the hash table
    std::list<std::pair<KeyType, ValueType> > table[hashGroups];    //list of 
};



template<typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::isEmpty() const{
    int sum;
    for (int i = 0; i < hashGroups; i++) {
        sum += table[i].size();
    }

    if (sum == 0) {
        return true;
    }
    return false;
}
#endif