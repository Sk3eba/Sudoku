#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"
#include "Numbers.h"
#include "Game.h"
#include "Solver.h"

static void drawBoardNumbers(sf::RenderTarget& target, const Game& game, const Grid& grid, const sf::Font& font)
{
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            int v = game.getCell({ c, r });
            if (v <= 0) continue;

            sf::FloatRect cellBounds = grid.getCellBounds({ c, r });

            sf::Text t(font);
            t.setString(std::to_string(v));
            // character size relative to cell height
            unsigned int charSize = static_cast<unsigned int>(cellBounds.size.y * 0.6f);
            t.setCharacterSize(charSize);
            t.setFillColor(sf::Color::Black);

            sf::FloatRect tb = t.getLocalBounds();
            t.setOrigin(tb.getCenter());
            t.setPosition(cellBounds.getCenter());

            target.draw(t);
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Sudoku");
    window.setFramerateLimit(60);

    // create grid on the left
    Grid grid({ 20.f, 30.f }, 540.f);
	sf::Font font("Roboto-Regular.ttf");
    Numbers numbers({ 20.f + 540.f + 20.f, 30.f }, 205.f, font);

    Game game;

    // generate puzzle with desired number of clues (e.g., 30)
    std::array<int, 81> puzzle = Solver::generatePuzzle(30);

    // solve puzzle to obtain authoritative solution (should be unique)
    std::array<int, 81> solution;
    Solver::solve(puzzle, solution);

    game.clear();
    for (int i = 0; i < 81; ++i)
    {
        if (puzzle[i] != 0)
        {
            sf::Vector2i cell(i % 9, i / 9);
            game.setCell(cell, puzzle[i]);
        }
    }


    sf::Vector2i selectedCell(-1, -1);

    while (window.isOpen())
    {
        // simple render loop (only handle window close so the window stays responsive)
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                    {
                        // convert the event pixel position to world coordinates
                        sf::Vector2f worldPos = window.mapPixelToCoords(mb->position);

                        sf::Vector2i cell = grid.cellAt(worldPos);
                        numbers.selectAtPosition(worldPos);
                        int selNum = numbers.getSelectedNumber();

                        if (cell.x >= 0 && cell.y >= 0)
                        {
                            selectedCell = cell;
                            grid.selectCell(selectedCell);

                            if (selNum != -1)
                            {
                                int flatIdx = selectedCell.y * 9 + selectedCell.x;
                                if (solution[flatIdx] == selNum && game.isValidMove(selectedCell, selNum))
                                    game.setCell(selectedCell, selNum);
                            }
                        }
                        else
                        {
                            // clicked outside grid (probably on numbers). If user clicked a number
                            // and there is a previously selected grid cell, place the number there.
                            if (selNum != -1 && selectedCell.x >= 0 && selectedCell.y >= 0)
                            {
                                // require that the chosen number matches the precomputed solution
                                int flatIdx = selectedCell.y * 9 + selectedCell.x;
                                if (solution[flatIdx] == selNum && game.isValidMove(selectedCell, selNum))
                                    game.setCell(selectedCell, selNum);
                            }
                        }

                    }
                }
                
            }
        }

        window.clear(sf::Color::White);

		grid.draw(window);

		drawBoardNumbers(window, game, grid, font);

        numbers.draw(window);

        window.display();
    }
}