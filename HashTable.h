#ifndef COVIDVISUALIZATION_HASHTABLE_H
#define COVIDVISUALIZATION_HASHTABLE_H

#include "Key.h"
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
using namespace std;

class HashTable{
private:
    std::unordered_map<std::string,Key*> hashTable;

public:
    void insertKey(Key* newKey){
        hashTable[newKey->getDate()] = newKey;
    }

    Key* findKey(std::string date){
        if(hashTable.find(date) == hashTable.end()){
            return nullptr;
        }
        else{
            return hashTable[date];
        }
    }

    void PrintTable()
    {
        for (auto iter = hashTable.begin() ; iter != hashTable.end() ; ++iter) {
            cout << "Date: " << iter->first << endl;
            for (int i = 0; i <iter->second->getStates().size() ; ++i) {
                cout << "State: " << iter->second->getStates().at(i).getName() << " Cases: " << iter->second->getStates().at(i).getCaseCount();
                cout << " Deaths: " << iter->second->getStates().at(i).getDeaths() << endl;
            }
        }
    }

    void PrintDate(std::string date)
    {
        cout << date << ":" << endl;

        for (int i = 0; i < hashTable[date]->getStates().size() ; ++i) {
            cout << setw(20) << "State: " << endl << setw(25) << hashTable[date]->getStates().at(i).getName()<< endl;

            for (int j = 0; j < hashTable[date]->getStates().at(i).getCounties().size() ; ++j) {
                cout << setw(30) << "County: " << hashTable[date]->getStates().at(i).getCounties().at(j).getName() << " Cases: " <<
                hashTable[date]->getStates().at(i).getCounties().at(j).getCaseCount() << " Deaths: " << hashTable[date]->getStates().at(i).getCounties().at(j).getDeaths()<< endl;
            }
        }
    }
    void insertData(std::vector<Key*>& keys){
      for(int i = 0; i < keys.size();i++){
        hashTable[keys.at(i)->getDate()] = newKey;
      }
    }

    /*void LoadStateData(const std::string& filepath) {
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
                string year, month, day, state, fips, s_cases, s_deaths;
                getline(stream, year, '-');
                getline(stream, month, '-');
                getline(stream, day, ',');

                getline(stream, state, ',');
                getline(stream, fips, ',');
                getline(stream, s_cases, ',');
                getline(stream, s_deaths, ',');

                //convert to necessary data type
                int deaths = stoi(s_deaths);
                int caseCount = stoi(s_cases);
                //convert date from string to int
                year.append(month);
                year.append(day);
                int date = stoi(year);


                //=====insert all the data into its key in the hashtable=====

                //check if key exists
                if(findKey(date) != nullptr && state != "Puerto Rico")   //if it does exist, add state to key in hashmap
                {
                    hashTable[date]->getStates().emplace_back(state,caseCount,deaths,fips);
                }
                else   // if it doesnt, create key and add state and insert into map
                {
                    if(state != "Puerto Rico") {
                        insertKey(new Key(date));
                        hashTable[date]->getStates().emplace_back(state, caseCount, deaths,fips);
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
                string year, month, day, county, state, fips, s_cases, s_deaths;
                getline(stream, year, '-');
                getline(stream, month, '-');
                getline(stream, day, ',');

                getline(stream, county, ',');
                getline(stream, state, ',');
                getline(stream, fips, ',');
                getline(stream, s_cases, ',');
                getline(stream, s_deaths, ',');
                if (state != "Puerto Rico") {
                    //cout << month << "-" << day << ": " <<county << ", " << state << ", " << fips << ", " << s_cases << ", " << s_deaths << ", " << endl;

                    //convert to necessary data type
                    int deaths = stoi(s_deaths);
                    int caseCount = stoi(s_cases);
                    //convert date from string to int
                    year.append(month);
                    year.append(day);
                    int date = stoi(year);


                    //=====insert all the data into its key in the hashtable=====

                    //all dates should exist bc state data is already loaded

                    //go to key in the table and find the state(not puerto rico)

                    int index = hashTable[date]->StateIndex(state);

                    //add county data to that state in its vector
                    hashTable[date]->getStates().at(index).addCounty(County(county, caseCount, deaths, fips));

                }
            }
        }
        file.close();
    }*/

};

#endif //COVIDVISUALIZATION_HASHTABLE_H