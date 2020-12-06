#ifndef COVIDVISUALIZATION_KEYLIST_H
#define COVIDVISUALIZATION_KEYLIST_H

#include "Key.h"
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <iostream>

class KeyList{
private:
    int preKeyIndex;
    std::unordered_set<std::string> dates;
    std::vector<Key*> keys;

public:

    KeyList();

    std::vector<Key*> GetData();

    Key* FindKey(const std::string& date);

    void LoadStateData(const std::string& filepath);

    void LoadCountyData(const std::string& filepath);
};


#endif //COVIDVISUALIZATION_KEYLIST_H