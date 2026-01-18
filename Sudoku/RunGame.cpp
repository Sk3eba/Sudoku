#include "RunGame.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"
#include "Numbers.h"
#include "Game.h"
#include "Solver.h"
#include "ToolButtons.h"
#include <SFML/Audio.hpp>

void RunGame::drawBoardNumbers(sf::RenderTarget& target, const Game& game, const Grid& grid, const sf::Font& font, int selectedNumber, std::array<int, 81> puzzle)
{
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            int v = game.getCell({ c, r });


            sf::FloatRect cellBounds = grid.getCellBounds({ c, r });
            if (puzzle[r * 9 + c] != 0)
            {
                sf::RectangleShape hl;
                hl.setSize({ cellBounds.size });
                hl.setPosition(cellBounds.position);
                hl.setFillColor(sf::Color(0, 0, 0, 30));
                target.draw(hl);
            }
            if (selectedNumber > 0 && v == selectedNumber && (game.isValidMove({ c, r }, v) || puzzle[r * 9 + c] != 0))
            {
                sf::RectangleShape hl;
                hl.setSize({ cellBounds.size });
                hl.setPosition(cellBounds.position);
                hl.setFillColor(sf::Color(173, 216, 230, 160));
                target.draw(hl);
            }

            if (!game.isValidMove({ c, r }, v) && puzzle[r * 9 +c] == 0) {
                sf::RectangleShape wrong;
                wrong.setSize({ cellBounds.size });
                wrong.setPosition(cellBounds.position);
                wrong.setFillColor(sf::Color(233, 49, 54, 211));
                target.draw(wrong);
            }

            if (v > 0) {
                sf::Text t(font);
                t.setString(std::to_string(v));
                unsigned int charSize = static_cast<unsigned int>(cellBounds.size.y * 0.6f);
                t.setCharacterSize(v == 0 ? 0 : charSize);
                t.setFillColor(sf::Color::Black);

                sf::FloatRect tb = t.getLocalBounds();
                t.setOrigin(tb.getCenter());
                t.setPosition(cellBounds.getCenter());

                target.draw(t);
            }
            else
            {
                // Draw pencil marks for empty cells
                unsigned int smallCharSize = static_cast<unsigned int>(cellBounds.size.y * 0.2f);
                float cellW = cellBounds.size.x / 3.0f;
                float cellH = cellBounds.size.y / 3.0f;

                for (int num = 1; num <= 9; ++num)
                {
                    if (game.hasCandidate({ c, r }, num))
                    {
                        int gridX = (num - 1) % 3;
                        int gridY = (num - 1) / 3;

                        sf::Text pencilText(font);
                        pencilText.setString(std::to_string(num));
                        pencilText.setCharacterSize(smallCharSize);
                        pencilText.setFillColor(sf::Color(100, 100, 100));

                        sf::FloatRect pencilBounds = pencilText.getLocalBounds();
                        pencilText.setOrigin(pencilBounds.getCenter());

                        float posX = cellBounds.position.x + cellW * gridX + cellW * 0.5f;
                        float posY = cellBounds.position.y + cellH * gridY + cellH * 0.5f;
                        pencilText.setPosition({ posX, posY });

                        target.draw(pencilText);
                    }
                }
            }
        }
    }
}

void RunGame::runGame(int clues)
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Sudoku");
    window.setFramerateLimit(60);

    Grid grid({ 20.f, 30.f }, 540.f);
    sf::Font font("Roboto-Regular.ttf");

    const sf::Vector2f numbersPos = { 20.f + 540.f + 20.f, 30.f };
    const float numbersSize = 205.f;
    Numbers numbers(numbersPos, numbersSize, font);

    const sf::Vector2f toolsPos = { numbersPos.x, numbersPos.y + numbersSize + 12.f };
    ToolButtons tools(toolsPos, 48.f, font);

    sf::Texture okTexture("ok.png");
    sf::Sprite okSprite(okTexture);
    okSprite.setTexture(okTexture);
    sf::Vector2u texSize = okTexture.getSize();
    okSprite.setOrigin(sf::Vector2f(static_cast<float>(texSize.x), static_cast<float>(texSize.y)));
    const sf::Vector2f okPosition = { numbersPos.x + numbersSize, numbersPos.y + 2.5f * numbersSize };
    okSprite.setPosition(okPosition);

    Game game;
    game.clear();

    std::array<int, 81> puzzle = Solver::generatePuzzle(clues);

    std::array<int, 81> solution;
    Solver::solve(puzzle, solution);

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
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                    {
                        // convert the event pixel position to world coordinates
                        sf::Vector2f worldPos = window.mapPixelToCoords(mb->position);

                        tools.selectAtPosition(worldPos);
                        if (tools.restartPressed())
                        {
                            puzzle = Solver::generatePuzzle(clues);
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
                            selectedCell = { -1, -1 };
                            grid.selectCell(selectedCell);
                            continue;
                        }

                        sf::Vector2i cell = grid.cellAt(worldPos);

                        int selNumBefore = numbers.getSelectedNumber();

                        if (!(cell.x >= 0 && cell.y >= 0))
                            numbers.selectAtPosition(worldPos);
                        int selNum = (cell.x >= 0 && cell.y >= 0) ? selNumBefore : numbers.getSelectedNumber();


                        if (cell.x >= 0 && cell.y >= 0 && puzzle[cell.y*9+cell.x] != 0) {
                            grid.selectCell({ -1, -1 });
                            break;
                        }

                        bool pencilMode = tools.isPencilActive();

                        if (cell.x >= 0 && cell.y >= 0)
                        {
                            selectedCell = cell;
                            grid.selectCell(selectedCell);

                            if (selNum != -1)
                            {
                                if (pencilMode)
                                {
                                    game.toggleCandidate(selectedCell, selNum);
                                }
                                else
                                {
                                    game.setCell(selectedCell, selNum);
                                    game.clearCandidates(selectedCell);
                                }
                                
                            }
                            
                        }
                        else
                        {
                            grid.selectCell({ -1, -1 });
                            if (!pencilMode && selNum != -1 && selectedCell.x >= 0 && selectedCell.y >= 0)
                            {
                                game.setCell(selectedCell, selNum);
                                selectedCell = { -1, -1 };
                                grid.selectCell(selectedCell);
                            }
                            
                        }
                        if (numbers.getSelectedNumber() > 0) {
                            selectedCell.x = -1;
                            selectedCell.y = -1;
                            grid.selectCell(selectedCell);
                        }
                        
                    }
                }

            }

            if (event->is<sf::Event::KeyPressed>())
            {
                if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
                {
                    int num = -1;
                    switch (kp->code)
                    {
                    case sf::Keyboard::Key::Num1: case sf::Keyboard::Key::Numpad1: num = 1; break;
                    case sf::Keyboard::Key::Num2: case sf::Keyboard::Key::Numpad2: num = 2; break;
                    case sf::Keyboard::Key::Num3: case sf::Keyboard::Key::Numpad3: num = 3; break;
                    case sf::Keyboard::Key::Num4: case sf::Keyboard::Key::Numpad4: num = 4; break;
                    case sf::Keyboard::Key::Num5: case sf::Keyboard::Key::Numpad5: num = 5; break;
                    case sf::Keyboard::Key::Num6: case sf::Keyboard::Key::Numpad6: num = 6; break;
                    case sf::Keyboard::Key::Num7: case sf::Keyboard::Key::Numpad7: num = 7; break;
                    case sf::Keyboard::Key::Num8: case sf::Keyboard::Key::Numpad8: num = 8; break;
                    case sf::Keyboard::Key::Num9: case sf::Keyboard::Key::Numpad9: num = 9; break;
                    default: break;
                    }
                    grid.selectCell({ -1, -1 });
                    if (num != -1)
                    {
                        int index = num - 1;
                        numbers.setSelectedCell({ index % 3, index / 3 });

                        if (selectedCell.x >= 0 && selectedCell.y >= 0)
                        {
                            bool pencilMode = tools.isPencilActive();

                            if (pencilMode)
                            {
                                game.toggleCandidate(selectedCell, num);
                            }
                            else
                            {
                                game.setCell(selectedCell, num);
                                game.clearCandidates(selectedCell);
                            }
                            selectedCell = { -1, -1 };
                            grid.selectCell(selectedCell);
                            numbers.setSelectedCell({ -1, -1 });
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        grid.draw(window);

        drawBoardNumbers(window, game, grid, font, numbers.getSelectedNumber(), puzzle);

        if (game.isSolved() && okTexture.getSize().x > 0)
        {
            window.draw(okSprite);
        }

        numbers.draw(window);
        tools.draw(window);
        window.display();
    }
}

