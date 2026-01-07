#pragma once

#include <array>
#include <SFML/System.hpp>

// Simple Sudoku game model (logic only).
// - Board is 9x9, stored row-major (0 = empty).
// - Provides setters/getters, move validation, and board validity/solved checks.
class Game
{
public:
    Game();

    // Clear board (set all cells to 0).
    void clear();

    // Set a cell to a value (1..9) or 0 to clear.
    // Returns true if the value was stored (coordinates/value valid).
    // Does NOT modify the value if coordinates/value are invalid.
    // This does not enforce immutability of given puzzle cells; caller must manage that.
    bool setCell(sf::Vector2i cell, int value);

    // Read value at cell (0..9). If out-of-range returns -1.
    int getCell(sf::Vector2i cell) const;

    // Check if placing 'value' into 'cell' would be valid according to Sudoku rules
    // (no duplicate in same row, column or 3x3 block). Does not change board.
    // value == 0 is always valid (clearing).
    bool isValidMove(sf::Vector2i cell, int value) const;

    // Check full board validity (no duplicates among non-zero values).
    bool isBoardValid() const;

    // Returns true when board is fully filled (no zeros) and valid.
    bool isSolved() const;

    // Access raw board as flat array (row-major).
    const std::array<int, 81>& rawBoard() const { return m_cells; }

    // --- Pencil / candidate API (UI-only notes, do not affect solution)
    // Toggle candidate number (1..9) for a cell. If already present it is removed.
    void toggleCandidate(sf::Vector2i cell, int number);
    // Get bitmask of candidates for a cell (bits 1..9).
    unsigned short getCandidatesMask(sf::Vector2i cell) const;
    // Clear all candidates for a cell.
    void clearCandidates(sf::Vector2i cell);

private:
    std::array<int, 81> m_cells;
    std::array<unsigned short, 81> m_candidates;

    static bool inRangeCell(sf::Vector2i cell);
    static int indexFromCell(sf::Vector2i cell);

    // helpers used by validation routines
    bool checkRowValid(int row) const;
    bool checkColValid(int col) const;
    bool checkBlockValid(int blockRow, int blockCol) const;
};