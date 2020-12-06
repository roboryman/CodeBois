#include "HashTable.h"

HashTable::HashTable()= default;

void HashTable::InsertKey(Key* newKey){
    hashTable[newKey->GetDate()] = newKey;
}

Key* HashTable::FindKey(const std::string& date){
    if(hashTable.find(date) == hashTable.end()){
        return nullptr;
    }
    else{
        return hashTable[date];
    }
}

void HashTable::PrintTable()
{
    for (auto & iter : hashTable) {
        cout << "Date: " << iter.first << endl;
        for (int i = 0; i < iter.second->GetStates()->size() ; ++i) {
            cout << "State: " << iter.second->GetStates()->at(i)->GetName() << " Cases: " << iter.second->GetStates()->at(i)->GetCaseCount();
            cout << " Deaths: " << iter.second->GetStates()->at(i)->GetDeaths() << endl;
        }
    }
}

void HashTable::PrintDate(const std::string& date)
{
    cout << date << ":" << endl;

    for (int i = 0; i < hashTable[date]->GetStates()->size() ; ++i) {
        cout << setw(20) << "State: " << endl << setw(25) << hashTable[date]->GetStates()->at(i)->GetName()<< endl;

        for (int j = 0; j < hashTable[date]->GetStates()->at(i)->GetCounties()->size() ; ++j) {
            cout << setw(30) << "County: " << hashTable[date]->GetStates()->at(i)->GetCounties()->at(j)->GetName() << " Cases: " << hashTable[date]->GetStates()->at(i)->GetCounties()->at(j)->GetCaseCount() << " Deaths: " << hashTable[date] ->GetStates()->at(i)->GetCounties()->at(j)->GetDeaths()<< endl;
        }
    }
}

//add data from datasets
void HashTable::InsertData(std::vector<Key*> keys){
    for(auto & key : keys){
        hashTable[key->GetDate()] = key;
    }
}