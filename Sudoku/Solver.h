#pragma once

#include <array>

class Solver
{
public:
    //returns true when solvable and writes the completed board into outSolution.
    static bool solve(const std::array<int, 81>& board, std::array<int, 81>& outSolution);

    //generate a fully solved Sudoku board, uses randomized backtracking.
    static std::array<int, 81> generateSolvedBoard();

    static std::array<int, 81> generatePuzzle(int clues = 30);

private:

    static bool solveBacktrack(std::array<int, 81>& board, bool randomize);

    static bool isValidPlacement(const std::array<int, 81>& board, int idx, int value);

    static int countSolutions(std::array<int, 81>& board, int limit);
};

