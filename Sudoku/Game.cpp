#include "Game.h"

Game::Game()
{
    clear();
}

void Game::clear()
{
    m_cells.fill(0);
}

bool Game::setCell(sf::Vector2i cell, int value)
{
    if (!inRangeCell(cell)) return false;
    if (value < 0 || value > 9) return false;
    m_cells[indexFromCell(cell)] = value;
    return true;
}

int Game::getCell(sf::Vector2i cell) const
{
    if (!inRangeCell(cell)) return -1;
    return m_cells[indexFromCell(cell)];
}

bool Game::isValidMove(sf::Vector2i cell, int value) const
{
    if (!inRangeCell(cell)) return false;
    if (value < 0 || value > 9) return false;

    if (value == 0) return true;

    int row = cell.y;
    for (int c = 0; c < 9; ++c)
    {
        if (c == cell.x) continue;
        int v = m_cells[row * 9 + c];
        if (v == value) return false;
    }

    int col = cell.x;
    for (int r = 0; r < 9; ++r)
    {
        if (r == cell.y) continue;
        int v = m_cells[r * 9 + col];
        if (v == value) return false;
    }

    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int rr = blockRow + r;
            int cc = blockCol + c;
            if (rr == row && cc == col) continue;
            int v = m_cells[rr * 9 + cc];
            if (v == value) return false;
        }
    }

    return true;
}

bool Game::checkRowValid(int row) const
{
    bool seen[10] = {};
    for (int c = 0; c < 9; ++c)
    {
        int v = m_cells[row * 9 + c];
        if (v == 0) continue;
        if (seen[v]) return false;
        seen[v] = true;
    }
    return true;
}

bool Game::checkColValid(int col) const
{
    bool seen[10] = {};
    for (int r = 0; r < 9; ++r)
    {
        int v = m_cells[r * 9 + col];
        if (v == 0) continue;
        if (seen[v]) return false;
        seen[v] = true;
    }
    return true;
}

bool Game::checkBlockValid(int blockRow, int blockCol) const
{
    bool seen[10] = {};
    int startR = blockRow * 3;
    int startC = blockCol * 3;
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int v = m_cells[(startR + r) * 9 + (startC + c)];
            if (v == 0) continue;
            if (seen[v]) return false;
            seen[v] = true;
        }
    }
    return true;
}

bool Game::isBoardValid() const
{
    for (int r = 0; r < 9; ++r)
        if (!checkRowValid(r)) return false;

    for (int c = 0; c < 9; ++c)
        if (!checkColValid(c)) return false;

    for (int br = 0; br < 3; ++br)
        for (int bc = 0; bc < 3; ++bc)
            if (!checkBlockValid(br, bc)) return false;

    return true;
}

bool Game::isSolved() const
{
    for (int i = 0; i < 81; ++i)
        if (m_cells[i] == 0) return false;

    return isBoardValid();
}

bool Game::inRangeCell(sf::Vector2i cell)
{
    return cell.x >= 0 && cell.x < 9 && cell.y >= 0 && cell.y < 9;
}

int Game::indexFromCell(sf::Vector2i cell)
{
    return cell.y * 9 + cell.x;
}

void Game::toggleCandidate(sf::Vector2i cell, int number)
{
    if (!inRangeCell(cell)) return;
    if (number < 1 || number > 9) return;
    int idx = indexFromCell(cell);
    unsigned short bit = static_cast<unsigned short>(1u << number);
    if (m_candidates[idx] & bit)
        m_candidates[idx] &= static_cast<unsigned short>(~bit);
    else
        m_candidates[idx] |= bit;
}

unsigned short Game::getCandidatesMask(sf::Vector2i cell) const
{
    if (!inRangeCell(cell)) return 0;
    return m_candidates[indexFromCell(cell)];
}

void Game::clearCandidates(sf::Vector2i cell)
{
    if (!inRangeCell(cell)) return;
    m_candidates[indexFromCell(cell)] = 0;
}