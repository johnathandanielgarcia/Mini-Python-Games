/*
key components:
- constructor: sets up welcome window with blue background and text elements 
- text management: centers text for display
- input handling:
    - accept only alphabetical letters
    - enforce 10 character limit
    - formats name with first letter capitalized, rest lowercase
    - processes backspace to delete character
- event processing: handles window events including closing 
- name validation: ensures entered name meets requirements 
- game flow: returns entered name to main program when Enter is pressed 
*/

#include "WelcomeWindow.h"
#include <iostream>
#include <cctype>

void WelcomeWindow::setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

WelcomeWindow::WelcomeWindow(int width, int height) : playerName("") {
    window.create(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close); // create window

    if (!font.loadFromFile("font.ttf")) { // load font 
        cerr << "Failed to load font!" << endl;
        return;
    }

    // set up welcome text 
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width / 2.0f, height / 2.0f - 150);

    // set up 'enter name' text
    enterNameText.setFont(font);
    enterNameText.setString("Enter your name: ");
    enterNameText.setCharacterSize(20);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setStyle(sf::Text::Bold);
    setText(enterNameText, width / 2.0f, height / 2.0f - 75);

    // set up input text
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    setText(inputText, width / 2.0f, height / 2.0f - 45);
}

string WelcomeWindow::run() {
    sf::Color backgroundColor(0, 0, 255); // set background color to blue

    // main loop
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return ""; // return an empty string when window closed
            }

            // added this bc the code block below using unicode '13' for return/enter did not work
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Enter) {
                    // cout << "Enter key pressed via KeyPressed event" << endl;
                    if (!playerName.empty()) {
                        // cout << "Returning name: " << playerName << endl; // debugging
                        window.close();
                        return playerName;
                    }
                }
                // if (event.key.code == sf::Keyboard::Backspace) {
                    // if (!playerName.empty()) {
                        // playerName.pop_back();
                    // }
                // }
            }

            if (event.type == sf::Event::TextEntered) { 
                if (event.text.unicode == 8) { // backspace
                    if (!playerName.empty()) {
                        playerName.pop_back();
                    }
                }
                else if (event.text.unicode == 13) { // enter
                    if (!playerName.empty()) {
                        window.close();
                        return playerName;
                    }
                }
                else if (isalpha(event.text.unicode) && playerName.length() < 10) { // only add alphabetical letters 
                    char inputChar = static_cast<char>(event.text.unicode);
                    if (playerName.empty()) {
                        playerName += toupper(inputChar);
                    } else {
                        playerName += tolower(inputChar);
                    }
                }
            }
        }
        // add cursor to text field 
        inputText.setString(playerName + "|");
        setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);

        window.clear(backgroundColor); // clear blue background 

        // draw window elements
        window.draw(welcomeText);
        window.draw(enterNameText);
        window.draw(inputText);

        window.display(); // display window 
    }
    return playerName;
}









