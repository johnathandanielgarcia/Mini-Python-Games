/*
purpose: represents a single tile, encapsulating all the properties and behaviors of individual game tiles 

implementation:
- maintains state of each tile (mine/not mine, revealed/hidden, flagged/not flagged)
- tracks position (row, col) within game board
- stores the number of adjacent mines
- keeps references to adjacent tiles for recursive revealing 
- provides drawing functionalities for different tile states
- handles special modes (debig mode, revealed-only mode)
*/

#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

class Tile {
private:
    int row;
    int col;
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
    sf::Sprite hiddenSprite;
    sf::Sprite revealedSprite;
    sf::Sprite mineSprite;
    sf::Sprite flagSprite;
    sf::Sprite numberSprite;
    vector<Tile*> adjacentTiles;

public:
    Tile(int row, int col, const sf::Texture& hiddenTexture, const sf::Texture& revealedTexture);

    // setters 
    void setPosition(float x, float y);
    void setMine(bool flag);
    void setRevealed(bool flag);
    void toggleFlag();
    void setFlag(bool value);
    void setAdjacentMines(int count);
    void addAdjacentTile(Tile* tile);

    // getters 
    int getRow() const;
    int getCol() const;
    bool getMine() const;
    bool getRevealed() const;
    bool getFlagged() const;
    int getAdjacentMines() const;
    const vector<Tile*>& getAdjacentTiles() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    // setters for sprites 
    void setMineSprite(const sf::Texture& texture);
    void setFlagSprite(const sf::Texture& texture);
    void setNumberSprite(const sf::Texture& texture, int number);

    void draw(sf::RenderWindow& window, bool debugMode);
    void drawRevealed(sf::RenderWindow& window);
};




#endif