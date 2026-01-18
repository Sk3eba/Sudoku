#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Grid.h"

class RunGame
{
public:
	void drawBoardNumbers(sf::RenderTarget& target, const Game& game, const Grid& grid, const sf::Font& font, int selectedNumber, std::array<int, 81> puzzle, std::array<int, 81> solution, bool showValidation);
	void runGame(int clues);
};

