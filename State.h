#ifndef COVIDVISUALIZATION_STATE_H
#define COVIDVISUALIZATION_STATE_H

#include "County.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

class State{
private:
    std::string name;
    std::vector<County> counties;
    int caseCount;
    int deaths;
public:
    State(){
        name = "";
        caseCount = 0;
        deaths = 0;
    }
    State(std::string name){
        this->name= name;
        caseCount = 0;
        deaths = 0;
    }
    std::string getName(){
        return name;
    }
    void addCounty(County newCounty){
        deaths += newCounty.getDeaths();
        caseCount += newCounty.getCaseCount();
        counties.push_back(newCounty);
    }
    std::vector<County> getCounties(){
        return counties;
    }
};
#endif //COVIDVISUALIZATION_STATE_H
