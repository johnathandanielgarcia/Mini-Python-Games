/*
purpose: represents main game window for game
handles ga,e board, UI elements, and all game logic

Implementations:
- creates and manages grid of Tile objects representing game board
- handles user interactions (mouse clicks, button pressed)
- manages game state (running, paused, won, lost)
- controls UI elements (buttons, timers)
- implements core game logic
    - random mine placement
    - revealing tiles and adjacent empty tiles
    - flag replacement and removal
    - victory / defeat conditions 
    - timer & pause/play functionality 
*/

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Tile.h"
#include "LeaderboardWindow.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <chrono>
using namespace std;

class GameWindow {
private:
    // basic settings 
    sf::RenderWindow window;
    int width;
    int height;
    int colCount;
    int rowCount;
    int mineCount;
    string playerName;

    // manage game state
    bool gameOver;
    bool gameWon;
    bool debugMode;
    bool paused;
    int flagCount;

    // timer implementation 
    chrono::time_point<chrono::high_resolution_clock> startTime;
    chrono::time_point<chrono::high_resolution_clock> pauseTime;
    int elapsedSeconds;
    bool timerRunning;

    // resources
    sf::Font font;
    map<string, sf::Texture> textures;

    // game board 
    vector<vector<Tile>> tiles;

    // UI elem
    sf::Sprite faceButton;
    sf::Sprite debugButton;
    sf::Sprite pauseButton;
    sf::Sprite leaderboardButton;
    vector<sf::Sprite> counterDigits;
    vector<sf::Sprite> timerDigits;
    sf::Sprite minusSign;

    // load resources & init game
    void loadTextures();
    void setupBoard();
    void placeMines();
    void calculateAdjacentMines();

    // helper methods 
    void revealTile(int x, int y);
    void toggleFlag(int x, int y);
    void checkVictory();
    void gameDefeat();
    void resetGame();
    void updateCounter();
    void updateTimer();
    void openLeaderboard(bool checkVictory);

    void drawBoard();
    void drawUI();
    void drawDigits(int value, vector<sf::Sprite>& digitSprites, bool showMinus = false);

public:
    GameWindow(int width, int height, int colCount, int rowCount, int mineCount, const string& playerName);
    void run(); // main loop function 
};





#endif