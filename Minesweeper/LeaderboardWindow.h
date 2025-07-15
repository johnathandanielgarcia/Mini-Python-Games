/*
purpose: implements leaderboard window for game

implementation overview:
- creates separate window to display the game's leaderboard 
- reads and writes player records from/to leaderboard.txt file 
- manages the leaderboard entries (player names and times)
- sorts entries by completion time 
- updates the leaderboard when a player achieves a new high score
- displays leaderboard in given format
- maintains only top 5 scores 
- highlights new records with an asterick (*)
*/

#ifndef LEADERBOARDWINDOW_H
#define LEADERBOARDWINDOW_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using namespace std;

class LeaderboardWindow {
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text leaderboardText;

    struct LeaderboardEntry {
        string time;
        string name;
        bool isNew;
    };

    vector<LeaderboardEntry> entries;

    void setText(sf::Text& text, float x, float y);
    void loadLeaderboard();
    void saveLeaderboard();
    string formatLeaderboard();
public:
    LeaderboardWindow(int width, int height);
    void run();
    bool checkAndUpdateLeaderboard(const string& playerName, int timeInSeconds);
};

#endif

