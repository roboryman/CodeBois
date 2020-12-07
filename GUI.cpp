#include <fstream>
#include <string>
#include <functional>
#include <iostream>
#include <random>
#include <chrono>
#include "GUI.h"
#include "TextureManager.h"
#include "libs/lodepng/lodepng.h"
#include "libs/libheatmap/heatmap.h"
#include "libs/libheatmap/colorschemes/RdGy.h"
#include "libs/libheatmap/colorschemes/Spectral.h"

/*
*
*                          GUI.cpp
*   --------------------------------------------------------
*   Implementation of our GUI. Handles SFML sprites and
*   SFGUI objects. Renders and encodes heatmaps.
*
*/

/*
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define NANOSVG_IMPLEMENTATION
#include "libs/nanosvg/nanosvg.h"
*/

using std::cout;
using std::endl;
using std::bind;
using std::ifstream;
using std::to_string;

using namespace std::chrono;

// Constructor for the GUI. This means setting default values, loading the
// dataset data into a KeyList, setting textures and properties of the
// sprites, adding UI elements to the hierarchy, and finally initializing
// the map.
GUI::GUI() {
    playActive = false;
    displayCases = true;
    stepTime = DEF_STEP_TIME;
    startDate = DATASET_BEGIN;
    endDate = DATASET_END;
    currentDate = DATASET_BEGIN;
    keyList = new KeyList();
    hashTable = new HashTable();
    bplusTree = nullptr;

    // Load datasets into KeyList for insertion into data structure later
    keyList->LoadStateData(DATASET_STATES_FILENAME);
    keyList->LoadCountyData(DATASET_COUNTIES_FILENAME);

    /*
    // This would parse the SVG and get each layer (county) position for
    // placement later.
    NSVGimage* image = NULL;
    image = nsvgParseFromFile("resources/textures/map.svg", "px", 96);
    ofstream pos;
    pos.open(POSITIONS_FILENAME);
    for (NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next) {
        float w = (shape->bounds[2]) - (shape->bounds[0]);
        float h = (shape->bounds[3]) - (shape->bounds[1]);
        string name = "resources/textures/counties/";
        name.append(shape->id);
        name.append(".png");

        pos << name << "\n"; // first line: texture path
        pos << (shape->bounds[0]) << "\n"; // second line: X pos
        pos << (shape->bounds[1]) << "\n"; // third line: Y pos
    }
    pos.close();
    nsvgDelete(image);
    */


    /*
     *                           *
     *  SETUP SFGUI UI ELEMENTS  *
     *                           *
    */

    // Read in each county layer's center position for placement on to the
    // map. Each county is a sprite, and has a FIPS code associated with it.
    ifstream positions(POSITIONS_FILENAME);
    string texturePath;
    string x;
    string y;
    if(positions.is_open()) {
        while(getline(positions, texturePath)) {
            getline(positions, x); // Get the "x" coordinate of the couny
            getline(positions, y); // Get the "y" coordinate of the county

            // Create and set up the county sprite
            sf::Sprite countySprite;
            countySprite.setTexture(
                TextureManager::GetTexture("resources/" + texturePath)
            );
            countySprite.setPosition(stof(x)+MAP_X, stof(y)+MAP_Y);
            counties.push_back(make_pair(   countySprite,
                                            texturePath.substr(19, 5)));

            string fips = texturePath.substr(19, 5);
            countyPositions[fips] = make_pair(stoi(x), stoi(y));
        }
        positions.close();
    }

    borders.setTexture(TextureManager::GetTexture(BORDERS_FILENAME));
    borders.setPosition(50.3291f+MAP_X, 23.3305f+MAP_Y);
    metrophobic.loadFromFile(METROPHOBIC_FILENAME);

    title.setFont(metrophobic);
    title.setString("COVID-19 Transmission Map");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color(140, 91, 91));
    title.setStyle(sf::Text::Bold);
    title.setPosition(30, 10);

    monthDay.setFont(metrophobic);
    monthDay.setCharacterSize(72);
    monthDay.setFillColor(sf::Color(140, 91, 91));
    monthDay.setStyle(sf::Text::Bold);
    monthDay.setPosition(600 - monthDay.getLocalBounds().width/2, 760);

    year.setFont(metrophobic);
    year.setCharacterSize(24);
    year.setFillColor(sf::Color(140, 91, 91));
    year.setPosition(600 - year.getLocalBounds().width/2, 845);
    
    info.setFont(metrophobic);
    info.setString( "Version 1.0\n"
                    "Data Tuples: 799K\n"
                    "Last Dataset Entry: " + endDate + "\n"
                    "CodeBois. All Rights Reserved." );
    info.setCharacterSize(14);
    info.setFillColor(sf::Color(138, 123, 123));
    info.setPosition(970, 17);

    heatmap.setTexture(TextureManager::GetTexture(HEATMAP_FILENAME));
    heatmap.setPosition(MAP_X, MAP_Y);

    time = sf::seconds(0);


    /*                           
     *                           *
     *  SETUP SFGUI UI ELEMENTS  *
     *                           *
    */

    dsToggleButton = sfg::Button::Create("Using Hash Table");
    dsToggleButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        bind(&GUI::ToggleDataStructure, this)
    );
    dsToggleButton->SetPosition(sf::Vector2f(891, 774));
    dsToggleButton->SetRequisition(sf::Vector2f(259, 76));

    displayTypeButton = sfg::Button::Create("Displaying Cases");
    displayTypeButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        bind(&GUI::ToggleDisplayType, this)
    );
    displayTypeButton->SetPosition(sf::Vector2f(50, 774));
    displayTypeButton->SetRequisition(sf::Vector2f(259, 76));

    steLabel = sfg::Label::Create(" Step Time");
    seLabel = sfg::Label::Create("Start Date");
    eeLabel = sfg::Label::Create("  End Date");

    stepTimeEntry = sfg::Entry::Create(to_string(stepTime));
    stepTimeEntry->SetRequisition(sf::Vector2f(85, 0));
    startEntry = sfg::Entry::Create(startDate);
    startEntry->SetRequisition(sf::Vector2f(85, 0));
    endEntry = sfg::Entry::Create(endDate);
    endEntry->SetRequisition(sf::Vector2f(85, 0));

    ste = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.0f);
    ste->Pack(steLabel);
    ste->Pack(stepTimeEntry);

    se = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.0f);
    se->Pack(seLabel);
    se->Pack(startEntry);

    ee = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.0f);
    ee->Pack(eeLabel);
    ee->Pack(endEntry);

    settingsBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
    settingsBox->SetPosition(sf::Vector2f(50, 670));
    settingsBox->Pack(ste);
    settingsBox->Pack(se);
    settingsBox->Pack(ee);

    applyButton = sfg::Button::Create("Apply");
    applyButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        bind(&GUI::ApplySettings, this)
    );
    applyButton->SetPosition(sf::Vector2f(215, 670));
    applyButton->SetRequisition(sf::Vector2f(95, 88));

    stateLabel = sfg::Label::Create("---");
    stateCasesLabel = sfg::Label::Create("--- Cases");
    stateDeathsLabel = sfg::Label::Create("--- Deaths");
    div = sfg::Separator::Create();
    countyLabel = sfg::Label::Create("--- County");
    countyCasesLabel = sfg::Label::Create("--- Cases");
    countyDeathsLabel = sfg::Label::Create("--- Deaths");

    stateLabel->SetClass("importantData");
    countyLabel->SetClass("importantData");

	box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    box->Pack(stateLabel);
    box->Pack(stateCasesLabel);
    box->Pack(stateDeathsLabel);
	box->Pack(div);
    box->Pack(countyLabel);
    box->Pack(countyCasesLabel);
    box->Pack(countyDeathsLabel);

	statsWindow = sfg::Window::Create();
	statsWindow->SetTitle("Statistics");
	statsWindow->Add(box);
    statsWindow->Show(false);

    fbButton = sfg::Button::Create("<==");
    fbButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        bind(&GUI::GotoFirst, this)
    );
    fbButton->SetPosition(sf::Vector2f(500, 720));
    fbButton->SetRequisition(sf::Vector2f(50, 50));

    playButton = sfg::Button::Create("Play");
    playButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        bind(&GUI::TogglePlay, this)
    );
    playButton->SetPosition(sf::Vector2f(575, 720));
    playButton->SetRequisition(sf::Vector2f(50, 50));

    ffButton = sfg::Button::Create("==>");
    ffButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        bind(&GUI::GotoLast, this)
    );
    ffButton->SetPosition(sf::Vector2f(650, 720));
    ffButton->SetRequisition(sf::Vector2f(50, 50));

    activeLabel1 = sfg::Label::Create("Active Data Structure");
    activeLabel2 = sfg::Label::Create("Hash Table");
    activeLabel2->SetClass("importantData");

    loadedLabel1 = sfg::Label::Create("Loaded in");
    loadedLabel2 = sfg::Label::Create("--- ms");
    loadedLabel2->SetClass("importantData");

    traversalLabel1 = sfg::Label::Create("Traversed this day in");
    traversalLabel2 = sfg::Label::Create("--- ms");
    traversalLabel2->SetClass("importantData");

    activeDiv = sfg::Separator::Create();
    loadedDiv = sfg::Separator::Create();

    timingBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
    timingBox->SetPosition(sf::Vector2f(13, 25));
    timingBox->Pack(activeLabel1);
    timingBox->Pack(activeLabel2);
    timingBox->Pack(activeDiv);
    timingBox->Pack(loadedLabel1);
    timingBox->Pack(loadedLabel2);
    timingBox->Pack(loadedDiv);
    timingBox->Pack(traversalLabel1);
    timingBox->Pack(traversalLabel2);

    timingFrame = sfg::Frame::Create("Timing");
    timingFrame->SetPosition(sf::Vector2f(1002, 600));
    timingFrame->Add(timingBox);

    desktop.SetProperty(".importantData", "Color", sf::Color(140, 91, 91));

    desktop.Add(dsToggleButton);
    desktop.Add(displayTypeButton);
    desktop.Add(settingsBox);
    desktop.Add(applyButton);
    desktop.Add(fbButton);
    desktop.Add(playButton);
    desktop.Add(ffButton);
    desktop.Add(timingFrame);
    desktop.Add(statsWindow);

    /*
     *                      *
     *  INITIALIZE THE MAP  *
     *                      *
     *
    */

    InsertAndTimeHashTable();
    GotoFirst();
}


// Redraws the GUI. Arguments are the render window to draw to, the position
// of the mouse for county hovering, and time elapsed for updating the
// desktop. Clears the window, draws all SFML and SFGUI elements, and finally
// displays to the render window.
void GUI::Redraw(sf::RenderWindow &rw, sf::Vector2f pos, sf::Time elapsed) {
	rw.clear(sf::Color(32, 32, 53)); // Clear the render window

    rw.draw(borders); // Draw the county borders

    // Draw each county. While doing this, check to see if the county is
    // actually being hovered over. If it is, then we want to display
    // statistics for that county and the state it belongs to.
    bool flag = false; // Flag indicates if a hover is occuring
    for(int i = 0; i < counties.size(); i++) {
        if (counties[i].first.getGlobalBounds().contains(pos) && !flag) {
            // County is being hovered over

            counties[i].first.setColor(sf::Color::Red); // Set color to red
            flag = true; // Set hover indicator to true

            // Pull the key for the current date from the active data structure
            Key *key;
            if(hashTable !=nullptr) {
                key = hashTable->FindKey(currentDate);
            }
            else {
                key = bplusTree->FindKey(currentDate);
            }

            if(key == nullptr) {
                cout << "Unable to find a key for date: " << \
                        currentDate << endl;
            }

            // Need to search for the county in our Key to provide statistics.
            // The county may not be in the dataset yet, so a search could
            // fail. Otherwise, set the labels to their respective data.
            bool statsFlag = false; // Indicates if we found the county
            for(auto &state : *key->GetStates()) {
                string stateName = state->GetName();

                // Only displaying the continental U.S. so skip these pests
                if( stateName == "Alaska" || stateName == "Guam" ||
                    stateName == "Hawaii" || stateName == "Virgin Islands" ||
                    stateName == "Northern Mariana Islands") {
                    continue;
                }

                for(auto &county : *state->GetCounties()) {
                    if(counties[i].second == county->GetFips()) {
                        // Located the correct county. Set the labels.

                        stateLabel->SetText(state->GetName());
                        stateCasesLabel->SetText(to_string(state->GetCaseCount()) + " Cases");
                        stateDeathsLabel->SetText(to_string(state->GetDeaths()) + " Deaths");
                        countyLabel->SetText(county->GetName() + " County");
                        countyCasesLabel->SetText(to_string(county->GetCaseCount()) + " Cases");
                        countyDeathsLabel->SetText(to_string(county->GetDeaths()) + " Deaths");
                        statsFlag = true;
                        break;
                    }
                }

                if(statsFlag) {
                    // Found the county; no need to search more states.
                    break;
                }
            }

            if(!statsFlag) {
                // Default values if no county was found
                stateLabel->SetText("n/a");
                stateCasesLabel->SetText("0 Cases");
                stateDeathsLabel->SetText("0 Deaths");
                countyLabel->SetText("n/a");
                countyCasesLabel->SetText("0 Cases");
                countyDeathsLabel->SetText("0 Deaths");
            }

        }
        else {
            // The county is not being hovered over, so no color is applied
            counties[i].first.setColor(sf::Color::White);
        }
        
        rw.draw(counties[i].first); // Finally... Draw the county
    }

    rw.draw(heatmap); // Draw the heatmap on top of the counties
    rw.draw(title); // Draw the title ("Covid-19 Transmission Map")
    rw.draw(monthDay); // Draw the current month and day
    rw.draw(year); // Draw the current year
    rw.draw(info); // Draw the upper-right info box
    
    // Only show the stats window if we are hovering over a county
    if(flag) {
        statsWindow->SetPosition(pos);
        statsWindow->Show(true);
    }
    else {
        statsWindow->Show(false);
    }

    desktop.Update(elapsed.asSeconds()); // Update the SFGUI Desktop
	sfgui.Display(rw); // Display SFGUI on the render window
	rw.display(); // Display the render window
}


// Event handler for SFGUI from the event loop in main().
void GUI::HandleEvent(sf::Event &event) {
    desktop.HandleEvent(event);
}


// Perform cleanup operations, i.e. when the user exits the program.
// We want to make sure any memory on the heap is deleted and that
// textures are cleared from our TextureManager.
void GUI::Cleanup() {
    delete hashTable;
    hashTable = nullptr;
    delete bplusTree;
    bplusTree = nullptr;

    TextureManager::ClearTextures();
}


// Ticks from the event loop in main(). If playActive is true, and stepTime
// has been surpassed, then we have to elapse a day into the future.
void GUI::LogicTick(sf::Time elapsed) {
    time += elapsed;
    if(time.asSeconds() > stepTime) {
        if(playActive) {
            DayElapsed();
        }
        time = sf::seconds(0); // Restart timer
    }
}


// Generates a heatmap using the data for each county in our dataset on
// the associated day. Heatmap positions are drawn at the county sprite
// centers.
void GUI::GenHeatmap() {
    auto start = high_resolution_clock::now();

    // Create the heatmap object and custom stamp
    static const size_t w = 989, h = 627;
    heatmap_t* hm = heatmap_new(w, h);
    heatmap_stamp_t* stamp = heatmap_stamp_gen(12);

    // Pull the key for this day 
    Key *key;
    if(hashTable != nullptr) {
        key = hashTable->FindKey(currentDate);
    }
    else {
        key = bplusTree->FindKey(currentDate);
    }

    // Look for an associated county FIPS in the data structure on this day.
    // We need to get the case count or death count.
    for(auto &state : *key->GetStates()) {
        string stateName = state->GetName();
        if( stateName == "Alaska" || stateName == "Guam" ||
            stateName == "Hawaii" || stateName == "Virgin Islands" ||
            stateName == "Northern Mariana Islands") {
            continue;
        }

        for(auto &county : *state->GetCounties()) {
            string fips = county->GetFips();
            int x = countyPositions[fips].first;
            int y = countyPositions[fips].second;

            if(x == 0 || y == 0) {
                continue;
            }

            if(displayCases) {
                heatmap_add_weighted_point_with_stamp(
                    hm, x, y, county->GetCaseCount(), stamp
                );
            }
            else {
                heatmap_add_weighted_point_with_stamp(
                    hm, x, y, county->GetDeaths(), stamp
                );
            }
        }
    }
    
    // Create a vector to hold the image data, w*h*4 bytes.
    std::vector<unsigned char> image(w*h*4);

    // Display the cases or deaths based on a max saturation constant
    if(displayCases) {
        heatmap_render_saturated_to(
            hm, heatmap_cs_Spectral_discrete, MAX_SATURATION_CASES, &image[0]
        );
    }
    else {
        heatmap_render_saturated_to(
            hm, heatmap_cs_RdGy_discrete, MAX_SATURATION_DEATHS, &image[0]
        );
    }
    
    heatmap_free(hm); // Free the memory used by the heatmap

    // Encode the heatmap and reload it as a texture
    lodepng::encode(HEATMAP_FILENAME, image, w, h);
    TextureManager::ClearTexture(HEATMAP_FILENAME);
    heatmap.setTexture(TextureManager::GetTexture(HEATMAP_FILENAME));

    // Update the timing labels for this heatmap generation
    auto stop = high_resolution_clock::now();
    auto difference = duration_cast<milliseconds>(stop - start);
    traversalLabel2->SetText(to_string(difference.count()) + " ms");
}


// Called when a day is elapsed. Currently, this only happens when playActive
// is true. Calculate the new date, generate the new heatmap, and update all
// date labels. If the new date is the endDate, we stop playing.
void GUI::DayElapsed() {
    // Calculate the year, month, and day for the new date
    string newYearStr = currentDate.substr(0, 4);
    string newMonthStr = currentDate.substr(5, 2);
    string newDayStr = currentDate.substr(8, 2);
    int newMonth = stoi(newMonthStr);
    int newDay = stoi(newDayStr) + 1;

    if (newDay > stoi(DayMonthMappings[stoi(newMonthStr)-1].first)) {
        newMonth += 1;
        newDay = 1;
    }

    if(newDay < 10) {
        newDayStr = "0" + to_string(newDay);
    }
    else {
        newDayStr = to_string(newDay);
    }

    if(newMonth < 10) {
        newMonthStr = "0" + to_string(newMonth);
    }
    else {
        newMonthStr = to_string(newMonth);
    }

    currentDate = newYearStr + "-" + newMonthStr + "-" + newDayStr;
    //cout << "New date: " << currentDate << endl;
    GenHeatmap(); // Generate the heatmap for the (new) currentDate

    if(newMonthStr==endDate.substr(5, 2) && newDayStr==endDate.substr(8, 2)) {
        StopPlay(); // Stop playing if we have reached the end, i.e. endDate
    }

    UpdateDateLabels(); // Make sure to update the date labels
}


// Toggles the current play state.
void GUI::TogglePlay() {
    if(playActive) {
        StopPlay();
    }
    else {
        StartPlay();
    }
}


// Starts playing through days. If the currentDate == endDate we just send
// the user back to the beginning. If not, set the button label and playActive.
void GUI::StartPlay() {
    if(currentDate == endDate) {
        GotoFirst();
    }

    playButton->SetLabel("Pause");
    playActive = true;
}

// Stop playing through days. Set button label and playActive.
void GUI::StopPlay() {
    playButton->SetLabel("Play");
    playActive = false;
}


// Apply user settings from the entry boxes. These are the step time,
// startDate, and endDate. Then, go to the beginning.
void GUI::ApplySettings() {
    cout << "Applied New Settings" << endl;

    stepTime = stof(stepTimeEntry->GetText().toAnsiString());
    startDate = startEntry->GetText().toAnsiString();
    endDate = endEntry->GetText().toAnsiString();

    GotoFirst();
}


// Go to the "beginning", i.e. stop playing if need be, set currentDate to
// startDate, update date labels, and generate the heatmap.
void GUI::GotoFirst() {
    StopPlay();
    currentDate = startDate;
    UpdateDateLabels();
    GenHeatmap();
}


// Go to the "end", i.e. stop playing if need be, set currentDate to
// endDate, update date labels, and generate the heatmap.
void GUI::GotoLast() {
    currentDate = endDate;
    UpdateDateLabels();
    GenHeatmap();
    StopPlay();
}


// Toggle what data structure to use. Basically, if the B+ tree is currently
// loaded with the keys, we want to get rid of it and use the Hash Table
// instead.
void GUI::ToggleDataStructure() {
    GotoFirst();

    if(bplusTree == nullptr) {
        // Switch to using the B+ Tree
        delete hashTable;
        hashTable = nullptr;
        bplusTree = new BplusTree();
        InsertAndTimeBplusTree();
    }
    else {
        // Switch to using the hash table
        delete bplusTree;
        bplusTree = nullptr;
        hashTable = new HashTable();
        InsertAndTimeHashTable();
    }
}


// Inserting the keyList into the B+ Tree and timing it.
void GUI::InsertAndTimeBplusTree() {
    auto start = high_resolution_clock::now();
    bplusTree->InsertData(keyList->GetData());

    auto stop = high_resolution_clock::now();
    auto difference = duration_cast<microseconds>(stop - start);
    
    activeLabel2->SetText("B+ Tree");
    loadedLabel2->SetText(to_string(difference.count()) + " us");
    dsToggleButton->SetLabel("Using B+ Tree");
    
}


// Inserting the keyList into the Hash Table and timing it.
void GUI::InsertAndTimeHashTable() {
    auto start = high_resolution_clock::now();
    hashTable->InsertData(keyList->GetData());

    auto stop = high_resolution_clock::now();
    auto difference = duration_cast<microseconds>(stop - start);
    
    activeLabel2->SetText("Hash Table");
    loadedLabel2->SetText(to_string(difference.count()) + " us");
    dsToggleButton->SetLabel("Using Hash Table");
    
}


// Toggle between displaying COVID-19 cases and deaths on the map.
void GUI::ToggleDisplayType() {
    GotoFirst();

    if(displayCases) {
        // Switch to displaying death
        displayTypeButton->SetLabel("Displaying Deaths");
    }
    else {
        // Switch to displaying cases
        displayTypeButton->SetLabel("Displaying Cases");
    }
    
    displayCases = !displayCases;
}


// Update any date labels to a human-readable form of currentDate.
void GUI::UpdateDateLabels() {
    string yearStr = currentDate.substr(0, 4);
    string monthStr = DayMonthMappings[stoi(currentDate.substr(5, 2))-1].second;
    string dayStr = to_string(stoi(currentDate.substr(8, 2)));
    monthDay.setString(monthStr + " " + dayStr);
    monthDay.setPosition(600 - monthDay.getLocalBounds().width/2, 760);
    year.setString(yearStr);
    year.setPosition(600 - year.getLocalBounds().width/2, 845);
}