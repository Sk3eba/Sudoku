#include "Solver.h"

#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <numeric>
#include <iostream>

bool Solver::solve(const std::array<int, 81>& board, std::array<int, 81>& outSolution)
{
    outSolution = board;
    return solveBacktrack(outSolution, true);
}

std::array<int, 81> Solver::generateSolvedBoard()
{
    std::array<int, 81> board{};
    board.fill(0);

    if (solveBacktrack(board, true))
        return board;

    std::array<int, 81> empty{};
    empty.fill(0);
    return empty;
}

std::array<int, 81> Solver::generatePuzzle(int clues)
{
    //generate a full solution first
    std::array<int, 81> full = generateSolvedBoard();
    if (std::all_of(full.begin(), full.end(), [](int v) { return v == 0; }))
        return full; //generation failed

	//17 = minimal amount of clues for unique solution
    clues = std::max(17, std::min(81, clues));

    //start from full board and try to remove numbers while preserving unique solution
    std::array<int, 81> puzzle = full;

    //build shuffled list of indices to attempt to remove
    std::vector<int> indices(81);
    std::iota(indices.begin(), indices.end(), 0);

	//rng for shuffling
    std::mt19937 rng(static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::shuffle(indices.begin(), indices.end(), rng);

    int keep = clues;
    int removed = 0;

    for (int idx : indices)
    {
        //if remaining cells equal keep, stop removing
        if (81 - removed <= keep)
            break;

        int saved = puzzle[idx];
        puzzle[idx] = 0;

        //check uniqueness: count solutions up to 2
        auto copy = puzzle;
        int sols = countSolutions(copy, 2);
        if (sols != 1)
        {
            //not unique (either 0 or multiple) -> restore
            puzzle[idx] = saved;
        }
        else
        {
            ++removed;
        }
    }
	
    return puzzle;
}

int Solver::countSolutions(std::array<int, 81>& board, int limit)
{
    //find first empty cell
    int idx = -1;
    for (int i = 0; i < 81; ++i)
    {
        if (board[i] == 0) { idx = i; break; }
    }

    if (idx == -1)
        return 1; //one complete solution found

    int count = 0;
    for (int val = 1; val <= 9; ++val)
    {
        if (isValidPlacement(board, idx, val))
        {
            board[idx] = val;
            count += countSolutions(board, limit - count);
            board[idx] = 0;

            if (count >= limit) //early exit when reaching limit
                return count;
        }
    }

    return count;
}

bool Solver::solveBacktrack(std::array<int, 81>& board, bool randomize)
{
    //find first empty cell
    int idx = -1;
    for (int i = 0; i < 81; ++i)
    {
        if (board[i] == 0) { idx = i; break; }
    }

    //no empty cells -> solved
    if (idx == -1) return true;

    //prepare candidate numbers 1..9
    std::vector<int> candidates(9);
    for (int i = 0; i < 9; ++i) candidates[i] = i + 1;

    if (randomize)
    {
		//rng for shuffling
        static thread_local std::mt19937 rng(static_cast<unsigned int>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()));
        std::shuffle(candidates.begin(), candidates.end(), rng);
    }

    for (int val : candidates)
    {
        if (isValidPlacement(board, idx, val))
        {
            board[idx] = val;
            if (solveBacktrack(board, randomize))
                return true;
            board[idx] = 0;
        }
    }

    return false;
}

bool Solver::isValidPlacement(const std::array<int, 81>& board, int idx, int value)
{
    const int row = idx / 9;
    const int col = idx % 9;

    //check row
    for (int c = 0; c < 9; ++c)
    {
        int i = row * 9 + c;
        if (i != idx && board[i] == value) return false;
    }

    //check column
    for (int r = 0; r < 9; ++r)
    {
        int i = r * 9 + col;
        if (i != idx && board[i] == value) return false;
    }

    //check 3x3 block
    const int blockRow = (row / 3) * 3;
    const int blockCol = (col / 3) * 3;
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int i = (blockRow + r) * 9 + (blockCol + c);
            if (i != idx && board[i] == value) return false;
        }
    }

    return true;
}
