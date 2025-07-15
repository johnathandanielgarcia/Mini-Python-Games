/*
purpose: main entry point for game
init and coordinates diff game windows and components 

implementation overview: 
- configuration loading: 
    - reads board dimensions and mine count from config.cfg file 
    - sets up default values if config is not found
    - validates configuration values to ensure the meet the minimum req

- game flow management: 
    - creates the welcome window to get player's name
    - init the main game window with proper dimensions 
    - passed player info between windows 

- error handling: 
    - handles missing files and invalid configurations 
    - provides debug output to help diagnose issues 
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "WelcomeWindow.h"
#include "GameWindow.h"
#include "LeaderboardWindow.h"


int main() {
    ifstream config("config.cfg");
    int colCount, rowCount, mineCount;

    // load config.cfg file is available 
    if (config.is_open()) {
        config >> colCount >> rowCount >> mineCount;
        config.close();
        // cout << "Read from config: columns=" << colCount << ", rows=" << rowCount 
                 // << ", mines=" << mineCount << endl; // debugging 
    } else {
        cerr << "Unable to open config file :(" << endl;
        colCount = 25; // set default values in case file cannot be read
        rowCount = 16;
        mineCount = 50;
    }

    // handling edge cases & ensuring proper size for all text/buttons on screen
    if (colCount < 22) colCount = 22;
    if (rowCount < 16) rowCount = 16;

    int totalTiles = colCount * rowCount;
    if (mineCount < 1) mineCount = 1;
    if (mineCount >= totalTiles) mineCount = totalTiles - 1;

    int width = colCount*32;
    int height = (rowCount * 32) + 100;
    // cout << "Window dimensions: " << width << "x" << height << endl; // debugging 

    // create welcome window
    WelcomeWindow welcomeWindow(width, height);

    // get player name 
    string playerName = welcomeWindow.run();

    if (playerName.empty()) {
        cout << "Empty player name." << endl;
        return 0;
    }


    // start game functionality after getting player name 
    GameWindow gameWindow(width, height, colCount, rowCount, mineCount, playerName);
    gameWindow.run();

    return 0;
}