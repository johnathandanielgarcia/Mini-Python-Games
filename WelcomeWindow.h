/*
purpose: implements initial welcome window of game
handles player name input before starting the actual game

implementation:
- creates first window when game launched
- displayes welcome message and prompts for player name input 
- handles keyboard input with validation:
    - limit to 10 characters
    - only alphabetical characters
    - formats name (first letter capitalized, rest lowercase)
- processes enter key to proceed with game
- displays cursor 
returns player name to main program
*/

#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <string>
using namespace std;

class WelcomeWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text welcomeText;
    sf::Text enterNameText;
    sf::Text inputText;
    string playerName;

    void setText(sf::Text& text, float x, float y);

public:
    WelcomeWindow(int width, int height);
    string run();

};



#endif