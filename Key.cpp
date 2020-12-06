#include "Key.h"



Key::Key(){
    date = "";
    preStateIndex = 0;
    left = nullptr;
    right = nullptr;
}

Key::Key(std::string date){
    this->date = std::move(date);
    preStateIndex = 0;
    left = nullptr;
    right = nullptr;
}

std::string Key::GetDate(){
    return date;
}

std::vector<State*>* Key::GetStates() {
    return &states;
}

int Key::StateIndex(const std::string& stateName){
    for (int i = 0; i < states.size() ; ++i) {
        if(states.at(i)->GetName() == stateName){return i;}
    }
    return -1;
}

int Key::FindState(const std::string& stateName){
    for(int i = preStateIndex; i < states.size(); i++){
        if(states.at(i)->GetName() == stateName){
            preStateIndex = i;
            return i;
        }
    }

    for(int i = 0; i < preStateIndex;i++){
        if(states.at(i)->GetName() == stateName){
            preStateIndex = i;
            return i;
        }
    }
    return -1;
}

