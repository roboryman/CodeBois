#pragma once

#include <vector>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>

#define MAX_SATURATION_CASES 100000
#define MAP_X 90.0f
#define MAP_Y 100.0f

using std::vector;

class GUI
{
    /* BEGIN SFML OBJECTS */
    vector<sf::Sprite> counties;
    sf::Sprite borders;
    sf::Font metrophobic;
    sf::Text title;
    sf::Text monthDay;
    sf::Text year;
    sf::Text info;
    sf::Sprite heatmap;
    sf::Clock clock;
    /* END SFML OBJECTS */


    /* BEGIN SFGUI OBJECTS */
    sfg::SFGUI sfgui;
    sfg::Desktop desktop;
    sfg::Button::Ptr dsToggleButton;
    sfg::Button::Ptr loadDatasetButton;
    sfg::Label::Ptr seLabel;
    sfg::Label::Ptr eeLabel;
    sfg::Entry::Ptr startEntry;
    sfg::Entry::Ptr endEntry;
    sfg::Box::Ptr se;
    sfg::Box::Ptr ee;
    sfg::Box::Ptr datesBox;
    sfg::Button::Ptr applyButton;
    sfg::Label::Ptr ss;
    sfg::Separator::Ptr div;
    sfg::Label::Ptr cs;
    sfg::Box::Ptr box;
    sfg::Window::Ptr statsWindow;
    /* END SFGUI OBJECTS */


    public:
        GUI();
        void Redraw(sf::RenderWindow &rw, sf::Vector2f pos);
        void HandleEvent(sf::Event &event);
        void Cleanup();

};