#ifndef COVIDVISUALIZATION_KEYLIST_H
#define COVIDVISUALIZATION_KEYLIST_H

#include "Key.h"
#include <unordered_set>

class KeyList{
  private:
  int preKeyIndex;

  public:
  unordered_set<string> dates;
  vector<Key*> keys;


  KeyList(){
    previousI = 0;
  }

  Key* findKey(string date){
    if(dates[date] == dates::end){
      return nullptr
    }
    else{
      for(int i = preKeyIndex; i < keys.size(); i++){
        if(keys.at(i).getDate()) == date){
          preKeyIndex = i;
          return keys.at(i);
        }
      }
      for(int i = 0; i < preKeyIndex;i++){
        if(keys.at(i).getDate()) == date){
          preKeyIndex = i;
          return keys.at(i);
        }
      }
    }
  }

  void LoadStateData(const std::string& filepath) {
    //opens the filepath into file
    ifstream file(filepath);

    if (file.is_open()) {
    //Get line from file and discard
    string lineFromFile;
    getline(file, lineFromFile);

      while (!file.eof()) {

        //get line from file
        getline(file, lineFromFile);

        //take line and put into stream
        istringstream stream(lineFromFile);

        //use getline and delimiters to get each element
        string date, state, fips, s_cases, s_deaths;

        getline(stream, date, ',');
        getline(stream, state, ',');
        getline(stream, fips, ',');
        getline(stream, s_cases, ',');
        getline(stream, s_deaths, ',');

        //convert to necessary data type
        int deaths = stoi(s_deaths);
        int caseCount = stoi(s_cases);

        /*=====insert all the data into its key in the list=====*/

        //check if key exists
        if(findKey(date) != nullptr && state != "Puerto Rico"){
          //if it does exist, add state to key in list
          findKey(date)->getStates().emplace_back(state,caseCount,deaths,fips);
        }
        else{   // if it doesnt, create key and add state and insert into list
          if(state != "Puerto Rico") {
            keys.emplace_back(new Key(date));
            dates.insert(date);
            findKey(date)->getStates().emplace_back(state, caseCount, deaths,fips);
          }
        }
      }
    }
    file.close();
  }

  void LoadCountyData(const std::string& filepath) {  
   //opens the filepath into file
    ifstream file(filepath);
    if (file.is_open()) {
      //Get line from file and discard
      string lineFromFile;
      getline(file, lineFromFile);
    
      while (!file.eof()) {
        //get line from file
        getline(file, lineFromFile);

        //take line and put into stream
        istringstream stream(lineFromFile);
        //use getline and delimiters to get each element
        string date, county, state, fips, s_cases, s_deaths;
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


          /*=====insert all the data into its key in the list=====*/

          //all dates should exist bc state data is already loaded

          if(findKey(date) != nullptr && findKey(date).findState(state) != nullptr){
            findKey(date)->findState(state).addCounty(County(county, caseCount, deaths,  fips));
          }else{ 
            std::cout << "key with date not found or state not found in key" << std::end;
          }

        }
      }
    }
    file.close();
  }
}


#endif //COVIDVISUALIZATION_KEYLIST_H