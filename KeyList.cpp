#include "KeyList.h"

KeyList::KeyList(){
    preKeyIndex = 0;
}

std::vector<Key*> KeyList::KeyList::GetData(){
    return keys;
}

Key* KeyList::FindKey(const std::string& date){
    if(dates.find(date) == dates.end()){
        return nullptr;
    }
    else{
        for(int i = preKeyIndex; i < keys.size(); i++){
            if(keys.at(i)->GetDate() == date){
                preKeyIndex = i;
                return keys.at(i);
            }
        }
        for(int i = 0; i < preKeyIndex;i++){
            if(keys.at(i)->GetDate() == date){
                preKeyIndex = i;
                return keys.at(i);
            }
        }
    }
    return nullptr;
}

void KeyList::LoadStateData(const std::string& filepath) {
    //opens the filepath into file
    std::ifstream file(filepath);

    if (file.is_open()) {
        //Get line from file and discard
        std::string lineFromFile;
        getline(file, lineFromFile);

        while (!file.eof()) {

            //get line from file
            getline(file, lineFromFile);

            //take line and put into stream
            std::istringstream stream(lineFromFile);

            //use getline and delimiters to get each element
            std::string date, state, fips, s_cases, s_deaths;

            getline(stream, date, ',');
            getline(stream, state, ',');
            getline(stream, fips, ',');
            getline(stream, s_cases, ',');
            getline(stream, s_deaths, ',');

            //convert to necessary data type
            int deaths = stoi(s_deaths);
            int caseCount = stoi(s_cases);

            //=====insert all the data into its key in the list=====

            //check if key exists
            if(FindKey(date) != nullptr && state != "Puerto Rico"){
                //if it does exist, add state to key in list
                FindKey(date)->GetStates().push_back(new State(state,caseCount,deaths,fips));
            }
            else{   // if it doesnt, create key and add state and insert into list
                if(state != "Puerto Rico") {
                    keys.push_back(new Key(date));
                    dates.insert(date);
                    FindKey(date)->GetStates().push_back(new State(state,caseCount,deaths,fips));
                }
            }
        }
    }
    file.close();
}

void KeyList::LoadCountyData(const std::string& filepath) {
    //opens the filepath into file
    std::ifstream file(filepath);
    if (file.is_open()) {
        //Get line from file and discard
        std::string lineFromFile;
        getline(file, lineFromFile);

        while (!file.eof()) {
            //get line from file
            getline(file, lineFromFile);

            //take line and put into stream
            std::istringstream stream(lineFromFile);
            //use getline and delimiters to get each element
            std::string date, county, state, fips, s_cases, s_deaths;
            getline(stream, date, ',');
            getline(stream, county, ',');
            getline(stream, state, ',');
            getline(stream, fips, ',');
            getline(stream, s_cases, ',');
            getline(stream, s_deaths, ',');
            if (state != "Puerto Rico") {
                //cout << month << "-" << day << ": " <<county << ", " << state << ", " << fips <<  ", " << s_cases << ", " << s_deaths << ", " << endl;

                //convert to necessary data type
                int deaths = stoi(s_deaths);
                int caseCount = stoi(s_cases);


                //=====insert all the data into its key in the list=====

                //all dates should exist bc state data is already loaded

                if(FindKey(date) != nullptr && FindKey(date)->FindState(state) != -1){
                    std::vector<State*> test = FindKey(date)->GetStates();
                    test.at(FindKey(date)->FindState(state))->AddCounty((new County(date,caseCount,deaths,fips)));
                }else{
                    std::cout << "key with date not found or state not found in key" << std::endl;
                }

            }
        }
    }
    file.close();
}