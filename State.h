#ifndef COVIDVISUALIZATION_STATE_H
#define COVIDVISUALIZATION_STATE_H

#include "County.h"
#include <vector>

class State{
private:
    std::string name;
    std::vector<County*> counties;
    int caseCount;
    int deaths;
    std::string fips;
public:
    State();

    State(std::string name, int cases, int deaths_, std::string fips);

    int GetCaseCount() const;

    int GetDeaths() const;

    std::string GetName();

    void AddCounty(County* newCounty);

    std::vector<County*>* GetCounties();
};
#endif //COVIDVISUALIZATION_STATE_H