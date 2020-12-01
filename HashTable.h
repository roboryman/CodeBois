#ifndef COVIDVISUALIZATION_HASHTABLE_H
#define COVIDVISUALIZATION_HASHTABLE_H

#include "Key.h"

class HashTable{
private:
    std::unordered_map<int,Key*> hashTable;

public:
    void insertKey(Key* newKey){
        hashTable[newKey->getDate()] = newKey;
    }
    Key* findKey(int date){
        if(hashTable.find(date) == hashTable.end()){
            return nullptr;
        }
        else{
            return hashTable[date];
        }
    }

};


#endif //COVIDVISUALIZATION_HASHTABLE_H
