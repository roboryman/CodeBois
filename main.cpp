#include "BplusTree.h"
#include "County.h"
#include "HashTable.h"
#include "State.h"
#include "Key.h"
#include "KeyList.h"
//#include "GUI.h"


/*
Project
    Visualizing the Spread of COVID-19 in the United States
Contributers
    Austin Padilla
    Frank Olivera
    Merrick Ryman
*/




int main() {
  /*
  sf::RenderWindow rw( sf::VideoMode(1200, 900), "COVID-19 Transmission Map", sf::Style::Titlebar | sf::Style::Close);
  rw.setFramerateLimit(60);

  GUI gui;
  gui.Redraw(rw, sf::Vector2f(0,0));

  // BEGIN MAIN UPDATE LOOP 
	while(rw.isOpen()) {
		sf::Event event;
    auto mouse = sf::Mouse::getPosition(rw);
    auto realPos = rw.mapPixelToCoords(mouse);
		while(rw.pollEvent(event))
    {
      gui.HandleEvent(event);

			if(event.type == sf::Event::Closed)
      {
				rw.close();
			}
		}
       gui.Redraw(rw, realPos);
	}

  gui.Cleanup(); */









    //Need to test below with dataset
    
    BplusTree* BplusTree = new class BplusTree();
    HashTable* HashTable = new class HashTable();
    KeyList* kekw = new KeyList();
    kekw->LoadStateData("Dataset/us-states.csv");
    kekw->LoadCountyData("Dataset/us-counties.csv");
    BplusTree->insertData(kekw->getData());
    HashTable->insertData(kekw->getData());


    
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
