
#include "State.h"


State::State(){
    name = "";
    caseCount = 0;
    deaths = 0;
}

State::State(std::string name, int cases, int deaths_, std::string fips){
    this->name = std::move(name);
    caseCount = cases;
    deaths = deaths_;
    this->fips = std::move(fips);
}

int State::GetCaseCount() const{
    return caseCount;
}

int State::GetDeaths() const{
    return deaths;
}

std::string State::GetName(){
    return name;
}
void State::AddCounty(County* newCounty){
    counties.push_back(newCounty);
}

std::vector<County*> State::GetCounties() {
return counties;
}

