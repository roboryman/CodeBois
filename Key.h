#ifndef COVIDVISUALIZATION_KEY_H
#define COVIDVISUALIZATION_KEY_H

#include "State.h"

class Key{
private:
    std::string date;
    std::vector<State> states;
    int preStateIndex;

public:
    Key* left;
    Key* right;

    Key(){
        std::string date = "";
        left = nullptr;
        right = nullptr;
    }

    Key(std::string date){
        this->date = date;
        left = nullptr;
        right = nullptr;
    }

    std::string getDate(){
        return date;
    }

    std::vector<State> &getStates() {
        return states;
    }

    int StateIndex(std::string stateName){
        for (int i = 0; i < states.size() ; ++i) {
            if(states.at(i).getName() == stateName){return i;}
        }
        return -1;
    }

    int findState(std::string stateName){
        for(int i = preStateIndex; i < states.size(); i++){
            if(states.at(i).getName() == stateName){
                preStateIndex = i;
                return i;
            }
        }

        for(int i = 0; i < preStateIndex;i++){
            if(states.at(i).getName() == stateName){
                preStateIndex = i;
                return i;
            }
        }
        return -1;
    }

};


#endif //COVIDVISUALIZATION_KEY_H
