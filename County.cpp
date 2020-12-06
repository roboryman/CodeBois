#include "County.h"


County::County(){
    this->name = "";
    this->caseCount = 0;
    this->deaths = 0;
}
County::County(std::string name, int caseCount, int deaths, std::string fips){
    this->name = std::move(name);
    this->caseCount = caseCount;
    this->deaths = deaths;
    this->fips = std::move(fips);
}
int County::GetDeaths() const{
    return deaths;
}
int County::GetCaseCount() const{
    return caseCount;
}
std::string County::GetName(){
    return name;
}