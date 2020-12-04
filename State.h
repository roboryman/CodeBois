#ifndef COVIDVISUALIZATION_STATE_H
#define COVIDVISUALIZATION_STATE_H

#include "County.h"
#include <vector>

class State{
private:
    std::string name;
    std::vector<County> counties;
    int caseCount;
    int deaths;
    std::string fips;
public:
    State(){
        name = "";
        caseCount = 0;
        deaths = 0;
    }
    
    State(std::string name, int cases, int deaths_, std::string fips){
        this->name = name;
        caseCount = cases;
        deaths = deaths_;
        fips = fips;
    }
    
    int getCaseCount() const {
        return caseCount;
    }

    int getDeaths() const {
        return deaths;
    }

    std::string getName(){
        return name;
    }

    void addCounty(County newCounty){
        counties.push_back(newCounty);
    }

    std::vector<County> getCounties(){
        return counties;
    }
};
#endif //COVIDVISUALIZATION_STATE_H
