#ifndef COVIDVISUALIZATION_HASHTABLE_H
#define COVIDVISUALIZATION_HASHTABLE_H

#include "Key.h"
#include <iomanip>
#include <iostream>
#include <unordered_map>

using namespace std;

class HashTable{
private:
    std::unordered_map<std::string, Key*> hashTable;

public:
    HashTable();

    void InsertKey(Key* newKey);

    Key* FindKey(const std::string& date);

    void PrintTable();

    void PrintDate(const std::string& date);

    void InsertData(std::vector<Key*> keys); //add data from datasets

};

#endif //COVIDVISUALIZATION_HASHTABLE_H