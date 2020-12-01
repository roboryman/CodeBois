#ifndef COVIDVISUALIZATION_COUNTY_H
#define COVIDVISUALIZATION_COUNTY_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

class County{
private:
    std::string name;
    int caseCount;
    int deaths;
public:
    County(){
        this->name = "";
        this->caseCount = 0;
        this->deaths = 0;
    }
    County(std::string name){
        this->name = name;
        this->caseCount = 0;
        this->deaths = 0;
    }
    int getDeaths(){
        return deaths;
    }
    int getCaseCount(){
        return caseCount;
    }
    std::string getName(){
        return name;
    }
};


#endif //COVIDVISUALIZATION_COUNTY_H
