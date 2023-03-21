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
    HashTable(int capacity);

    //DESTRUCTOR
    //~HashTable();

    //GETTERS
    bool containsKey(const KeyType& key) const; //checks to see if a particular key is present
    bool containsValue(const ValueType& value) const; //checks to tsee if a particular value is present
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
    static constexpr int hashGroups = 10;    //number of indices in the hash table
    std::list<std::pair<KeyType, ValueType> > table[hashGroups];
};

//CONSTRUCTORS
template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(){
    // constructor code here
}

template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(int capacity){
    // constructor code here
}

//DESTRUCTOR


//GETTERS
template<typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::containsKey(const KeyType& key) const {
    bool found = (std::find(table.begin(), table.end(), key) != table.end());
    return found;
}

//SETTERS
template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    if (!containsKey(key)) {
        table.insert(std::make_pair(key, value));
    }
}


#endif