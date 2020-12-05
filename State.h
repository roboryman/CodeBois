#ifndef COVIDVISUALIZATION_STATE_H
#define COVIDVISUALIZATION_STATE_H

#include "County.h"
#include <utility>
#include <vector>

class State{
private:
    std::string name;
    std::vector<County*>* counties;
    int caseCount;
    int deaths;
    std::string fips;
public:
    State(){
        counties = new std::vector<County*>;
        name = "";
        caseCount = 0;
        deaths = 0;
    }

    State(std::string name, int cases, int deaths_, std::string fips){
        counties = new std::vector<County*>;
        this->name = std::move(name);
        caseCount = cases;
        deaths = deaths_;
        this->fips = std::move(fips);
    }

    int getCaseCount() const{
        return caseCount;
    }

    int getDeaths() const{
        return deaths;
    }

    std::string getName(){
        return name;
    }

    void addCounty(County* newCounty){
        counties->push_back(newCounty);
    }

    std::vector<County*>* getCounties(){
        return counties;
    }
};
#endif //COVIDVISUALIZATION_STATE_H