#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "KeyList.h"
#include "HashTable.h"
#include "BplusTree.h"

/*
*
*                           GUI.h
*   --------------------------------------------------------
*   Header file for the GUI. Contains default values, paths
*   to texture files, mappings, and definitions for member
*   variables, member functions, SFML objects, SFGUI objects. 
*
*/

#define MAX_SATURATION_CASES 10000
#define MAX_SATURATION_DEATHS 1000
#define MAP_X 90.0f
#define MAP_Y 100.0f

#define DEF_STEP_TIME 0.5f

#define DATASET_BEGIN "2020-01-21"
#define DATASET_END "2020-11-30"

#define DATASET_STATES_FILENAME "resources/datasets/us-states.csv"
#define DATASET_COUNTIES_FILENAME "resources/datasets/us-counties.csv"
#define POSITIONS_FILENAME "resources/textures/counties/positions.txt"
#define BORDERS_FILENAME "resources/textures/borders.png"
#define METROPHOBIC_FILENAME "resources/fonts/Metrophobic.ttf"
#define HEATMAP_FILENAME "resources/textures/heatmap.png"
    

using std::vector;
using std::pair;
using std::string;
using std::unordered_map;
using std::pair;

static const pair<string, string> DayMonthMappings[12] =
{   {"31", "January"},
    {"29", "February"},
    {"31", "March"},
    {"30", "April"},
    {"31", "May"},
    {"30", "June"},
    {"31", "July"},
    {"31", "August"},
    {"30", "September"},
    {"31", "October"},
    {"30", "November"},
    {"31", "December"}  };

class GUI
{
    bool playActive;
    bool displayCases;
    float stepTime;
    string startDate;
    string endDate;
    string currentDate;
    KeyList *keyList;
    HashTable *hashTable;
    BplusTree *bplusTree;
    unordered_map<string, pair<int, int>> countyPositions;

    /* BEGIN SFML OBJECTS */
    vector<pair<sf::Sprite, string>> counties;
    sf::Sprite borders;
    sf::Font metrophobic;
    sf::Text title;
    sf::Text monthDay;
    sf::Text year;
    sf::Text info;
    sf::Sprite heatmap;
    sf::Time time;
    /* END SFML OBJECTS */


    /* BEGIN SFGUI OBJECTS */
    sfg::SFGUI sfgui;
    sfg::Desktop desktop;
    sfg::Button::Ptr dsToggleButton;
    sfg::Button::Ptr displayTypeButton;
    sfg::Label::Ptr steLabel;
    sfg::Label::Ptr seLabel;
    sfg::Label::Ptr eeLabel;
    sfg::Entry::Ptr stepTimeEntry;
    sfg::Entry::Ptr startEntry;
    sfg::Entry::Ptr endEntry;
    sfg::Box::Ptr ste;
    sfg::Box::Ptr se;
    sfg::Box::Ptr ee;
    sfg::Box::Ptr settingsBox;
    sfg::Button::Ptr applyButton;
    sfg::Label::Ptr stateLabel;
    sfg::Label::Ptr stateCasesLabel;
    sfg::Label::Ptr stateDeathsLabel;
    sfg::Separator::Ptr div;
    sfg::Label::Ptr countyLabel;
    sfg::Label::Ptr countyCasesLabel;
    sfg::Label::Ptr countyDeathsLabel;
    sfg::Box::Ptr box;
    sfg::Window::Ptr statsWindow;
    sfg::Button::Ptr fbButton;
    sfg::Button::Ptr playButton;
    sfg::Button::Ptr ffButton;
    sfg::Label::Ptr activeLabel1;
    sfg::Label::Ptr activeLabel2;
    sfg::Label::Ptr loadedLabel1;
    sfg::Label::Ptr loadedLabel2;
    sfg::Label::Ptr traversalLabel1;
    sfg::Label::Ptr traversalLabel2;
    sfg::Separator::Ptr activeDiv;
    sfg::Separator::Ptr loadedDiv;
    sfg::Box::Ptr timingBox;
    sfg::Frame::Ptr timingFrame;
    /* END SFGUI OBJECTS */

    void GenHeatmap();
    void DayElapsed();
    void TogglePlay();
    void StartPlay();
    void StopPlay();
    void ToggleDataStructure();
    void InsertAndTimeBplusTree();
    void InsertAndTimeHashTable();
    void ToggleDisplayType();
    void UpdateDateLabels();

    public:
        GUI();
        void Redraw(sf::RenderWindow &rw, sf::Vector2f pos, sf::Time elapsed);
        void HandleEvent(sf::Event &event);
        void Cleanup();
        void LogicTick(sf::Time elapsed);
        void ApplySettings();
        void GotoFirst();
        void GotoLast();
};