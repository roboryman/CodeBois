#include "BplusTree.h"
#include "County.h"
#include "HashTable.h"
#include "State.h"
#include "Key.h"


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
    sf::RenderWindow window(sf::VideoMode(1200, 900), "COVID-19 Visualization");
    window.setVerticalSyncEnabled(true);

      // game;
      //game.RedrawSprites(window);

      while (window.isOpen())
      {
          sf::Event event;
          while(window.pollEvent(event))
          {
              if(event.type == sf::Event::Closed)
              {
                  window.close();
              }
              else if(event.type == sf::Event::MouseButtonPressed)
              {
                  if(event.mouseButton.button == sf::Mouse::Left)
                  {
                      auto mouse = sf::Mouse::getPosition(window);
                      auto real_pos = window.mapPixelToCoords(mouse);
                      //game.LeftMouseButton(real_pos);
                  }
                  else if(event.mouseButton.button == sf::Mouse::Right)
                  {
                      auto mouse = sf::Mouse::getPosition(window);
                      auto real_pos = window.mapPixelToCoords(mouse);
                      //game.RightMouseButton(real_pos);
                  }
              }
          }
          //game.RedrawSprites(window);
      }

      //game.Cleanup();
      return 0;


    */




/*
    BplusTree* treetest = new BplusTree();
    for(int i = 0; i < 20;i++){
        treetest->insertKey(treetest->makeKey(i));
    }
    treetest->Transverse();
    treetest->LevelOrderTranserse();
    Key* testKey = treetest->findKey(4);
    if(testKey == nullptr){
        std::cout << "Not Found" << std::endl;
    }
    else{
        std::cout << "Found" << std::endl;
        std::cout << testKey->getDate() << std::endl;
    }*/

//Loading Data stuff
HashTable test;
test.LoadStateData("Dataset/us-states.csv");
test.LoadCountyData("Dataset/us-counties.csv");
test.PrintDate(20201127);


return 0;

}
