#include "KeyList.h"
#include "BplusTree.h"
#include "HashTable.h"
#include <chrono>

using namespace std::chrono;

/*
Project
    Visualizing the Spread of COVID-19 in the United States
Contributers
    Austin Padilla
    Frank Olivera
    Merrick Ryman
*/





int main() {

    //Need to test below with dataset
    KeyList* kekw = new KeyList();
    BplusTree* kek2 = new BplusTree;
    HashTable* kek3 = new HashTable();
    kekw->LoadStateData("resources/datasets/us-states.csv");
    //kek3->InsertData(kekw->GetData());
    //kek3->PrintTable();
    kek2->InsertData(kekw->GetData());
    kek2->Transverse();

    int test = 1;
    
    /*while(test >= 1){
      cin >> test;
      // start timing
      auto start = high_resolution_clock::now();
      if(test == 2){
        if(kek2 == nullptr){
          delete kek3;
          kek3 = nullptr;
          std::cout << "check BPLUS" << std::endl;
          kek2 = new BplusTree();
          kek2->InsertData(kekw->GetData());
        }
        else{
          delete kek2;
          kek2 = nullptr;
          std::cout << "check hash" << std::endl;
          kek3 = new HashTable();
          kek3->InsertData(kekw->GetData());
        }
      }
      // end timing here
      auto stop = high_resolution_clock::now();
      auto difference = duration_cast<microseconds>(stop - start);
      std::cout << difference.count() << std::endl;
      // update timing labels
    }*/
    
    

    //kekw->LoadStateData("resources/datasets/us-states.csv");
    ////kekw->LoadCountyData("resources/datasets/us-counties.csv");
    
    
    /*
    auto start = high_resolution_clock::now();
    //kek2->insertData(kekw->getData());
    Key* startdate = kek2->findKey("2020-10-08");
    startdate = startdate->right;
    auto stop = high_resolution_clock::now();
    auto difference = duration_cast<microseconds>(stop - start);
    std::cout << difference.count() << std::endl;
    kek2->LevelOrderTranserse();
    delete kek2;//made bplustree deconstructor to remove the left and right pointers of key taht it uses
    

    start = high_resolution_clock::now();
    kek3->insertData(kekw->getData());
    kek3->findKey("2020-10-08");
    stop = high_resolution_clock::now();
    difference = duration_cast<microseconds>(stop - start);
    std::cout << difference.count() << std::endl;
    */
    
   /*
   BplusTree* treetest = new BplusTree();
    for(int i = 10; i < 100;i++){
      treetest->insertKey(treetest->makeKey(std::to_string(i)));
    }
    treetest->Transverse();
    treetest->LevelOrderTranserse();
    Key* testKey = treetest->findKey("-1");
    if(testKey == nullptr){
        std::cout << "Not Found" << std::endl;
    }
    else{
        std::cout << "Found" << std::endl;
        std::cout << testKey->getDate() << std::endl;
    }
    */


return 0;

}
