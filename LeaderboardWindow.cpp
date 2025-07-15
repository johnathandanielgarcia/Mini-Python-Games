/*
key components:
- constructor: sets up leaderboard window and loads existing scores 
- text management: cemnters and formats textr for display
- file i/o: reads from and writes to the leaderboard.txt file
- entry sorting: arranges leaderboard entries by completion time
- data formatting: convert time values and format leaderboard text
- record checking: determine if a player's score qualifies for the leaderboard
- event handling: processes window events (primarily closing)
*/

#include "LeaderboardWindow.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

// helper to set text in center 
void LeaderboardWindow::setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

// constructor
LeaderboardWindow::LeaderboardWindow(int width, int height) {
    
    window.create(sf::VideoMode(width, height), "Minesweeper Leaderboard", sf::Style::Close); // create window 
    
    
    if (!font.loadFromFile("font.ttf")) { // make sure font loads correctly
        cerr << "Failed to load font!" << std::endl;
        return;
    }
    
    // set basic text 
    titleText.setFont(font);
    titleText.setString("LEADERBOARD");
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(titleText, width / 2.0f, height / 2.0f - 120);
    
    // set text on leaderboard 
    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(18);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold);
    
    // load the actual data for leaderboard 
    loadLeaderboard();
    
    // make text look good 
    leaderboardText.setString(formatLeaderboard());
    setText(leaderboardText, width / 2.0f, height / 2.0f + 20);
}

// loads leaderboard entries 
void LeaderboardWindow::loadLeaderboard() {
    entries.clear();
    
    std::ifstream file("leaderboard.txt");
    if (!file.is_open()) {
        cerr << "Failed to open leaderboard file! Creating new one." << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            LeaderboardEntry entry;
            entry.time = line.substr(0, commaPos);
            entry.name = line.substr(commaPos + 1);
            entry.isNew = false;
            entries.push_back(entry);
        }
    }
    
    file.close();
}

// save leaderboard entries to be loaded again 
void LeaderboardWindow::saveLeaderboard() {
    std::ofstream file("leaderboard.txt");
    if (!file.is_open()) {
        cerr << "Failed to open leaderboard file for writing!" << endl;
        return;
    }
    
    for (const auto& entry : entries) {
        file << entry.time << "," << entry.name << endl;
    }
    
    file.close();
}

// helper for aesthetics 
std::string LeaderboardWindow::formatLeaderboard() {
    std::ostringstream oss;
    
    for (size_t i = 0; i < entries.size(); ++i) {
        oss << i + 1 << ".\t" << entries[i].time << "\t" << entries[i].name;
        if (entries[i].isNew) {
            oss << "*";
        }
        if (i < entries.size() - 1) {
            oss << "\n\n";
        }
    }
    
    return oss.str();
}

bool LeaderboardWindow::checkAndUpdateLeaderboard(const string& playerName, int timeInSeconds) {
    // time to mm:ss
    int minutes = timeInSeconds / 60;
    int seconds = timeInSeconds % 60;
    ostringstream timeStream;
    timeStream << setw(2) << setfill('0') << minutes << ":" 
               << setw(2) << setfill('0') << seconds;
    string playerTime = timeStream.str();
    
    
    loadLeaderboard();
    
    // init new entry 
    LeaderboardEntry newEntry;
    newEntry.time = playerTime;
    newEntry.name = playerName;
    newEntry.isNew = true;
    
    
    entries.push_back(newEntry);
    
    // sort by time
    sort(entries.begin(), entries.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        return a.time < b.time;
    });
    
    
    if (entries.size() > 5) {
        entries.resize(5);
    }
    
    // Check if the new entry made it to the leaderboard
    bool madeLeaderboard = false;
    for (const auto& entry : entries) {
        if (entry.isNew) {
            madeLeaderboard = true;
            break;
        }
    }
    
    
    if (madeLeaderboard) {
        saveLeaderboard();
    }
    
    
    leaderboardText.setString(formatLeaderboard());
    setText(leaderboardText, window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20);
    
    return madeLeaderboard;
}

// main loop 
void LeaderboardWindow::run() {
    
    sf::Color backgroundColor(0, 0, 255); // blue
    
    
    while (window.isOpen()) {
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        
        
        window.clear(backgroundColor);
        
        
        window.draw(titleText);
        window.draw(leaderboardText);
        
        
        window.display();
    }
}