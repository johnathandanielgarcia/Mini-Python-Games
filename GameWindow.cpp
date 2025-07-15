/*
key components:
- contructor: sets up game window, board, and init all elememts 
- resource mangement: load textures, fonts, and set up sprites 
- board setup: creates tile grid, place mines, and calculate adjacent mines 
- game logic: revealing tiles, flagging, victory/defeat conditions 
- UI management: buttons, counter, window events 
- event handling: processes mouse clicks & window events 
- drawing: renders the game board and UI elements 
- timer & counter: manages game time tracking and mines remaining display 
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "GameWindow.h"

GameWindow::GameWindow(int width, int height, int colCount, int rowCount, int mineCount, const string& playerName)
    : width(width), height(height), colCount(colCount), rowCount(rowCount), mineCount(mineCount), playerName(playerName),
    gameOver(false), gameWon(false), debugMode(false), paused(false), flagCount(0), elapsedSeconds(0), timerRunning(false) {

    // output to verify constructor parameters (debugging)
    // std::cout << "GameWindow constructor called with:" << std::endl;
    // std::cout << "  width: " << width << std::endl;
    // std::cout << "  height: " << height << std::endl;
    // std::cout << "  colCount: " << colCount << std::endl;
    // std::cout << "  rowCount: " << rowCount << std::endl;
    // std::cout << "  mineCount: " << mineCount << std::endl;
    // std::cout << "  playerName: " << playerName << std::endl;

    window.create(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    srand(static_cast<unsigned int>(time(nullptr)));

    counterDigits.resize(3);
    timerDigits.resize(4);

    loadTextures();
    setupBoard();
    placeMines();
    calculateAdjacentMines();

    startTime = chrono::high_resolution_clock::now();
    timerRunning = true;
}

void GameWindow::loadTextures() {
    // load fonts 
    if (!font.loadFromFile("font.ttf")) {
        cerr << "Failed to load font :(" << endl;
        return;
    }

    // store file names for textures 
    vector<string> textureFiles = {
        "mine", "tile_hidden", "tile_revealed", "flag",
        "number_1", "number_2", "number_3", "number_4", "number_5", "number_6", "number_7", "number_8",
        "face_happy", "face_win", "face_lose", 
        "debug", "pause", "play", "leaderboard"
    };

    // load textures 
    for (const auto& file : textureFiles) {
        sf::Texture temp;
        if (temp.loadFromFile("images/" + file + ".png")) {
            textures[file] = temp;
        } else {
            cerr << "Failed to load texture: " << file << ".png" << endl;
        }
    }

    sf::Texture digitsTexture;
    if (digitsTexture.loadFromFile("images/digits.png")) {
        textures["digits"] = digitsTexture;
    } else {
        cerr << "Failed to load texture: digits.png" << endl;
    }


    // set up ui buttons 
    faceButton.setTexture(textures["face_happy"]);
    debugButton.setTexture(textures["debug"]);
    pauseButton.setTexture(textures["pause"]);
    leaderboardButton.setTexture(textures["leaderboard"]);

    // now we need to position the buttons 
    float faceX = (colCount / 2.0f) * 32 - 32;
    float faceY = 32 * (rowCount + 0.5f);
    float debugX = (colCount * 32) - 304;
    float debugY = 32 * (rowCount + 0.5f);
    float pauseX = (colCount * 32) - 240;
    float pauseY = 32 * (rowCount + 0.5f);
    float leaderboardX = (colCount * 32) - 176;
    float leaderboardY = 32 * (rowCount + 0.5f);

    faceButton.setPosition(faceX, faceY);
    debugButton.setPosition(debugX, debugY);
    pauseButton.setPosition(pauseX, pauseY);
    leaderboardButton.setPosition(leaderboardX, leaderboardY);

    for (size_t i = 0; i < counterDigits.size(); ++i) {
        counterDigits[i].setTexture(textures["digits"]);
        timerDigits[i].setTextureRect(sf::IntRect(0, 0, 21, 32));
    }

    for (size_t i = 0; i < timerDigits.size(); ++i) {
        timerDigits[i].setTexture(textures["digits"]);
        timerDigits[i].setTextureRect(sf::IntRect(0, 0, 21, 32));
    }

    minusSign.setTexture(textures["digits"]);
    minusSign.setTextureRect(sf::IntRect(10*21, 0, 21, 32));
    minusSign.setPosition(12, 32 * (rowCount + .5f) + 16);
}

void GameWindow::setupBoard() {
    tiles.resize(rowCount, vector<Tile>(colCount, Tile(0, 0, textures["tile_hidden"], textures["tile_revealed"])));

    // init every tile
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            float x = col * 32; // set x, y positions 
            float y = row * 32;

            tiles[row][col] = Tile(row, col, textures["tile_hidden"], textures["tile_revealed"]);
            tiles[row][col].setPosition(x, y);
            tiles[row][col].setMineSprite(textures["mine"]);
            tiles[row][col].setFlagSprite(textures["flag"]);
        }
    }

    // init adjacent tiles 
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            // check the 8 possible adj positions 
            for (int r = -1; r <= 1; ++r) {
                for (int c = -1; c <= 1; ++c) {
                    if (r == 0 && c == 0) continue; // skip tile

                    int adjRow = row + r; // get adj positons 
                    int adjCol = col + c;

                    if (adjRow >= 0 && adjRow < rowCount && adjCol >= 0 && adjCol < colCount) { // if the pos is valid
                        tiles[row][col].addAdjacentTile(&tiles[adjRow][adjCol]);
                    }
                }
            }
        }
    }

    flagCount = 0;
    for (size_t i = 0; i<counterDigits.size(); ++i) {
        counterDigits[i].setPosition(33 + i*21, 32 * (rowCount + 0.5f) + 16);
    }

    float minutesStartX = (colCount * 32) - 97;
    float secondsStartX = (colCount * 32) - 54;
    float digitsY = 32 * (rowCount + .5f) + 16;

    timerDigits[0].setPosition(minutesStartX, digitsY);
    timerDigits[1].setPosition(minutesStartX + 21, digitsY);
    timerDigits[2].setPosition(secondsStartX, digitsY);
    timerDigits[3].setPosition(secondsStartX + 21, digitsY);

    updateCounter();
}

void GameWindow::placeMines() {
    int minesPlaced = 0;

    while (minesPlaced < mineCount) {
        int randRow = rand() % rowCount; // using % so we dont overflow 
        int randCol = rand() % colCount;

        if (!tiles[randRow][randCol].getMine()) {
            tiles[randRow][randCol].setMine(true);
            minesPlaced++;
        }
    }
}

void GameWindow::calculateAdjacentMines() {
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            // count adjacent mines in every row x col
            if (!tiles[row][col].getMine()) {
                int count = 0;
                const auto& adjacentTiles = tiles[row][col].getAdjacentTiles();

                for (const auto& tile : adjacentTiles) { // same as saying "for tile in adjacent tiles" in python 
                    if (tile->getMine()) {
                        count++;
                    }
                }

                tiles[row][col].setAdjacentMines(count);

                // update num sprite to match count 
                if (count > 0) {
                    string textureName = "number_" + to_string(count);
                    if (textures.find(textureName) != textures.end()) {
                        tiles[row][col].setNumberSprite(textures[textureName], count);
                    }
                }
            }
        }
    }
}

void GameWindow::revealTile(int row, int col) {
    // if tile is revealed, flagged, or game over then we dont have to do anything, just return
    if (tiles[row][col].getRevealed() || tiles[row][col].getFlagged() || gameOver || paused) {
        return;
    }

    tiles[row][col].setRevealed(true); // otherwise, reveal tile 

    if (tiles[row][col].getMine()) { // if tile is mine, game over
        gameDefeat();
        return;
    }

    if (tiles[row][col].getAdjacentMines() == 0) { // if tile has no adjacent mines
        const auto& adjacentTiles = tiles[row][col].getAdjacentTiles(); // reveal all adjacent tiles 
        for (const auto& tile : adjacentTiles) {
            if (!tile->getRevealed() && !tile->getFlagged()) {
                revealTile(tile->getRow(), tile->getCol());
            }
        }
    }

    checkVictory(); // check if game has been won 
}

void GameWindow::toggleFlag(int row, int col) {
    // similar to above (if tile revealed, game paused, or game over -> do nothing)
    if (tiles[row][col].getRevealed() || gameOver || paused) {
        return;
    }

    bool wasFlagged = tiles[row][col].getFlagged();
    tiles[row][col].toggleFlag(); // toggle the flag

    if (wasFlagged) {
        flagCount--;
    } else {
        flagCount++;
    }

    updateCounter();
}

void GameWindow::checkVictory() {
    // to win, all non mine tiles should be revealed, so check for that
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            if (!tiles[row][col].getMine() && !tiles[row][col].getRevealed()) {
                return; // if the condition is true, then we still have not turned over every non mine tile
            }
        }
    }

    gameWon = true;
    gameOver = true;

    for (int row = 0; row < rowCount; ++row) { // auto flag all mines if game has been won
        for (int col = 0; col < colCount; ++col) {
            if (tiles[row][col].getMine() && !tiles[row][col].getFlagged()) {
                tiles[row][col].setFlag(true);
                flagCount++;
            }
        }
    }
    updateCounter();

    faceButton.setTexture(textures["face_win"]);
    timerRunning = false;
    openLeaderboard(true);
}

void GameWindow::gameDefeat() {
    gameOver = true;

    // if the game is over we can show all the tiles 
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            if (tiles[row][col].getMine()) {
                tiles[row][col].setRevealed(true);
            }
        }
    }
    faceButton.setTexture(textures["face_lose"]);
    timerRunning = false;
}

void GameWindow::resetGame() {
    // reset game settings
    gameOver = false;
    gameWon = false;
    debugMode = false;
    paused = false;
    flagCount = 0;
    elapsedSeconds = 0;

    faceButton.setTexture(textures["face_happy"]);

    pauseButton.setTexture(textures["pause"]);

    setupBoard();
    placeMines();
    calculateAdjacentMines();

    startTime = chrono::high_resolution_clock::now();
    timerRunning = true;
}

void GameWindow::updateCounter() {
    int remainingMines = mineCount - flagCount;
    drawDigits(remainingMines, counterDigits, true);
}

void GameWindow::updateTimer() {
    if (!timerRunning) return;

    auto currentTime = chrono::high_resolution_clock::now();
    auto elapsedDuration = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
    elapsedSeconds = elapsedDuration.count();

    if (elapsedSeconds > 999) {
        elapsedSeconds = 999;
    }

    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;

    timerDigits[0].setTextureRect(sf::IntRect((minutes/10) * 21, 0, 21, 32));
    timerDigits[1].setTextureRect(sf::IntRect((minutes % 10) * 21, 0, 21, 32));
    timerDigits[2].setTextureRect(sf::IntRect((seconds / 10) * 21, 0, 21, 32));
    timerDigits[3].setTextureRect(sf::IntRect((seconds % 10) * 21, 0, 21, 32));
}

void GameWindow::drawDigits(int value, vector<sf::Sprite>& digitSprites, bool showMinus) {
    bool isNegative = value < 0;

    if (isNegative) {
        value = -value;
    }

    for (int i = digitSprites.size() - 1; i >= 0; i--) {
        int digit = value % 10;
        digitSprites[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        value /= 10;
    }

    if (isNegative && showMinus) {
        window.draw(minusSign);
    }
}

void GameWindow::openLeaderboard(bool checkVictory) {
    bool wasTimerRunning = timerRunning;
    timerRunning = false;

    if (!paused) {
        pauseTime = chrono::high_resolution_clock::now();
    }

    bool originalPaused = paused;
    paused = true;

    int leaderboardWidth = (colCount * 16) + 50;
    int leaderboardHeight = (rowCount * 16) + 50;

    window.clear(sf::Color::White);
    drawBoard();
    drawUI();
    window.display();

    LeaderboardWindow leaderboardWindow(leaderboardWidth, leaderboardHeight);
    if (checkVictory && gameWon) {
        leaderboardWindow.checkAndUpdateLeaderboard(playerName, elapsedSeconds);
    }
    leaderboardWindow.run();

    paused = originalPaused;

    if (!paused && wasTimerRunning && !gameOver) {
        startTime += chrono::high_resolution_clock::now() - pauseTime;
        timerRunning = true;
    }
}

void GameWindow::drawBoard() {
    if (paused) {
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                tiles[row][col].drawRevealed(window);
            }
        }
    } else {
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                tiles[row][col].draw(window, debugMode);
            }
        }
    }
}

void GameWindow::drawUI() {
    window.draw(faceButton);

    if (!gameOver) {
        window.draw(debugButton);
    }

    if (!gameOver) {
        window.draw(pauseButton);
    }

    window.draw(leaderboardButton);

    for (const auto& digit : counterDigits) {
        window.draw(digit);
    }

    for (const auto& digit: timerDigits) {
        window.draw(digit);
    }
}

// MAIN LOOP !!
void GameWindow::run() {
    // cout << "GameWindow::run() method started" << endl; // debugging 

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                // if user clicks face button -> reset game
                if (faceButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    resetGame();
                }

                // if user clicks debug button -> set it to opposite 
                if (!gameOver && debugButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    debugMode = !debugMode;
                }

                // if user clicks pause button -> set it to opposite and update sprite
                if (!gameOver && pauseButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    paused = !paused;
                    if (paused) {
                        pauseButton.setTexture(textures["play"]);
                        pauseTime = chrono::high_resolution_clock::now();
                        timerRunning = false;
                    } else {
                        pauseButton.setTexture(textures["pause"]);
                        if (!gameOver) {
                            startTime += chrono::high_resolution_clock::now() - pauseTime;
                            timerRunning = true;
                        }
                    }
                }

                if (leaderboardButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    openLeaderboard(false);
                }

                // if user clicks a tile -> either review or flag based on left or right click 
                if (!paused && !gameOver) {
                    for (int row = 0; row < rowCount; ++row) {
                        for (int col = 0; col < colCount; ++col) {
                            if (tiles[row][col].getBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                                if (event.mouseButton.button == sf::Mouse::Left) {
                                    revealTile(row, col);
                                } else if (event.mouseButton.button == sf::Mouse::Right) {
                                    toggleFlag(row, col);
                                }
                            }
                        }
                    }
                }
            }
        }
        updateTimer();

        window.clear(sf::Color::White); // clear w white bg

        drawBoard();
        drawUI();
        window.display();

        /* draw all the tiles
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                tiles[row][col].draw(window, debugMode);
            }
        }
        

        // draw buttons
        window.draw(faceButton);
        window.draw(debugButton);
        window.draw(pauseButton);
        window.draw(leaderboardButton);

        // finally can display 
        window.display();
        */
    }
}