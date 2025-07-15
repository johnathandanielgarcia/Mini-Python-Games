/*
key components:
- contructor: init a tile with its position and basic sprites 
- state management: methods to set/get the tile's state (mine, revealed, flagged)
- sprite management: handlkes visual representation of diff tile states
- position handling: sets and retrieves tile's position on board
- adjacent tiles: manages connections to neighboring tiles for recursive revealing
- drawing: renders tile based on current state and game mode 

each tile maintains its own set of sprites and can display itself diff based on state. 
*/

#include "Tile.h"

Tile::Tile(int row, int col, const sf::Texture& hiddenTexture, const sf::Texture& revealedTexture)
    : row(row), col(col), isMine(false), isRevealed(false), isFlagged(false), adjacentMines(0) {
        hiddenSprite.setTexture(hiddenTexture);
        revealedSprite.setTexture(revealedTexture);
    }

void Tile::setPosition(float x, float y) {
    hiddenSprite.setPosition(x, y);
    revealedSprite.setPosition(x, y);
    mineSprite.setPosition(x, y);
    flagSprite.setPosition(x, y);
    numberSprite.setPosition(x, y);
}

void Tile::setMine(bool flag) {
    isMine = flag;
}

void Tile::setRevealed(bool flag) {
    isRevealed = flag;
    if (isRevealed) {
        isFlagged = false;
    }
}

void Tile::toggleFlag() {
    if (!isRevealed) {
        isFlagged = !isFlagged;
    }
}

void Tile::setFlag(bool value) {
    if (!isRevealed) {
        isFlagged = value;
    }
}

void Tile::setAdjacentMines(int count) {
    adjacentMines = count;
}

void Tile::addAdjacentTile(Tile* tile) {
    adjacentTiles.push_back(tile);
}

int Tile::getRow() const {
    return row;
}

int Tile::getCol() const {
    return col;
}

bool Tile::getMine() const {
    return isMine;
}

bool Tile::getRevealed() const {
    return isRevealed;
}

bool Tile::getFlagged() const {
    return isFlagged;
}

int Tile::getAdjacentMines() const {
    return adjacentMines;
}

const vector<Tile*>& Tile::getAdjacentTiles() const {
    return adjacentTiles;
}

sf::Vector2f Tile::getPosition() const {
    return hiddenSprite.getPosition();
}

sf::FloatRect Tile::getBounds() const {
    return hiddenSprite.getGlobalBounds();
}

void Tile::setMineSprite(const sf::Texture& texture) {
    mineSprite.setTexture(texture);
}

void Tile::setFlagSprite(const sf::Texture& texture) {
    flagSprite.setTexture(texture);
}

void Tile::setNumberSprite(const sf::Texture& texture, int number) {
    numberSprite.setTexture(texture);
}

void Tile::draw(sf::RenderWindow& window, bool debugMode) {
    if (isRevealed) {
        window.draw(revealedSprite);

        if (isMine) {
            window.draw(mineSprite);
        } else if (adjacentMines > 0) {
            window.draw(numberSprite);
        }
    } else {
        window.draw(hiddenSprite);

        if (isFlagged) {
            window.draw(flagSprite);
        }

        if (debugMode && isMine) {
            window.draw(mineSprite);
        }
    }
}

void Tile::drawRevealed(sf::RenderWindow& window) {
    window.draw(revealedSprite);
}