#pragma once

#include <array>

class Solver
{
public:
    // Solve given (possibly partial) board. board: row-major 9x9 (0 = empty).
    // Returns true when solvable and writes the completed board into outSolution.
    static bool solve(const std::array<int, 81>& board, std::array<int, 81>& outSolution);

    // Generate a fully solved (completed) Sudoku board. Uses randomized backtracking.
    // If generation fails (very unlikely), returns an array filled with zeros.
    static std::array<int, 81> generateSolvedBoard();

    static std::array<int, 81> generatePuzzle(int clues = 30);

private:
    // Backtracking solver that modifies board in-place. If randomize==true, numbers
    // tried at each empty cell are shuffled (useful for generating varied boards).
    static bool solveBacktrack(std::array<int, 81>& board, bool randomize);

    // Check whether placing `value` at linear index `idx` is valid (row/col/block).
    static bool isValidPlacement(const std::array<int, 81>& board, int idx, int value);

    static int countSolutions(std::array<int, 81>& board, int limit);
};

