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
    County(){
        this->name = "";
        this->caseCount = 0;
        this->deaths = 0;
    }
    County(std::string name, int caseCount, int deaths, std::string fips){
        this->name = std::move(name);
        this->caseCount = caseCount;
        this->deaths = deaths;
        this->fips = std::move(fips);
    }
    int getDeaths() const{
        return deaths;
    }
    int getCaseCount() const{
        return caseCount;
    }
    std::string getName(){
        return name;
    }
};


#endif //COVIDVISUALIZATION_COUNTY_H