#include "GUI.h"

/*
* Project 3 - COP3530
*    Visualizing the Spread of COVID-19 in the United States
*
* Contributers
*    Austin Padilla
*    Frank Olivera
*    Merrick Ryman
*/


int main() {
  sf::RenderWindow rw( sf::VideoMode(1200, 900), "COVID-19 Transmission Map", sf::Style::Titlebar | sf::Style::Close);
  rw.setFramerateLimit(60);

  GUI gui;

  /* BEGIN MAIN UPDATE LOOP */
  sf::Clock clock;
	while(rw.isOpen()) {
		sf::Event event;
    sf::Time elapsed = clock.restart();
    auto mouse = sf::Mouse::getPosition(rw);
    auto realPos = rw.mapPixelToCoords(mouse);
		while(rw.pollEvent(event)) {
      gui.HandleEvent(event);
			if(event.type == sf::Event::Closed) {
				rw.close();
			}
		}

    gui.LogicTick(elapsed);
    gui.Redraw(rw, realPos, elapsed);
	}

  gui.Cleanup();

	return 0;
}


//  https://github.com/nytimes/covid-19-data
//  https://github.com/roboryman/CodeBois/