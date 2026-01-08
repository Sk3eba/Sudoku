#pragma once

#include <array>
#include <SFML/System.hpp>

class Game
{
public:
    Game();

    //clear board
    void clear();

	//set value at cell 0-9, returns true if successful
    bool setCell(sf::Vector2i cell, int value);

    //read value at cell 0-9, if out-of-range returns -1.
    int getCell(sf::Vector2i cell) const;

    //checks if move is valid (no duplicates)
    bool isValidMove(sf::Vector2i cell, int value) const;

    //checks if board is valid
    bool isBoardValid() const;

	//returns true if board is completely solved
    bool isSolved() const;

	//access board as array of 81 integers (0 = empty)
    const std::array<int, 81>& rawBoard() const { return m_cells; }

    void toggleCandidate(sf::Vector2i cell, int number);

    unsigned short getCandidatesMask(sf::Vector2i cell) const;

    void clearCandidates(sf::Vector2i cell);

private:
    std::array<int, 81> m_cells;
    std::array<unsigned short, 81> m_candidates;

    static bool inRangeCell(sf::Vector2i cell);
    static int indexFromCell(sf::Vector2i cell);

    //helpers for board validation
    bool checkRowValid(int row) const;
    bool checkColValid(int col) const;
    bool checkBlockValid(int blockRow, int blockCol) const;
};