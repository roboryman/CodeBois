#ifndef COVIDVISUALIZATION_COUNTY_H
#define COVIDVISUALIZATION_COUNTY_H

#include <string>
#include <utility>

class County{
private:
    std::string name;
    int caseCount;
    int deaths;
    std::string fips;
public:
    County();

    County(std::string name, int caseCount, int deaths, std::string fips);

    int GetDeaths() const;

    int GetCaseCount() const;

    std::string GetName();

    std::string GetFips();
};


#endif //COVIDVISUALIZATION_COUNTY_H