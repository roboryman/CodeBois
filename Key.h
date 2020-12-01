#ifndef COVIDVISUALIZATION_KEY_H
#define COVIDVISUALIZATION_KEY_H

#include "State.h"
#include "County.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

class Key{
private:
    int date;
    std::vector<State> States;
public:
    Key* left;
    Key* right;
    Key(){
        int date = -1;
        left = nullptr;
        right = nullptr;
    }
    Key(int date){
        this->date = date;
        left = nullptr;
        right = nullptr;
    }
    int getDate(){
        return date;
    }
};


#endif //COVIDVISUALIZATION_KEY_H
