#ifndef COVIDVISUALIZATION_KEY_H
#define COVIDVISUALIZATION_KEY_H

#include <utility>

#include "State.h"

class Key{
private:
    std::string date;
    std::vector<State*> states;
    int preStateIndex;

public:
    Key();

    Key(std::string date);

    std::string GetDate();

    std::vector<State*>* GetStates();

    int StateIndex(const std::string& stateName);

    int FindState(const std::string& stateName);

};


#endif //COVIDVISUALIZATION_KEY_H