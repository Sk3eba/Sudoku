#include "RunGame.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"
#include "Numbers.h"
#include "Game.h"
#include "Solver.h"
#include "ToolButtons.h"
#include <SFML/Audio.hpp>

void RunGame::drawBoardNumbers(sf::RenderTarget& target, const Game& game, const Grid& grid, const sf::Font& font, int selectedNumber)
{
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            int v = game.getCell({ c, r });


            sf::FloatRect cellBounds = grid.getCellBounds({ c, r });

            if (selectedNumber > 0 && v == selectedNumber)
            {
                sf::RectangleShape hl;
                hl.setSize({ cellBounds.size });
                hl.setPosition(cellBounds.position);
                hl.setFillColor(sf::Color(173, 216, 230, 160));
                target.draw(hl);
            }

            if (v <= 0) continue;

            sf::Text t(font);
            t.setString(std::to_string(v));
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

void RunGame::runGame(int clues)
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Sudoku");
    window.setFramerateLimit(60);
    sf::Music music("music.wav");
    music.setVolume(0.f);
    float pitch = 1.f;
    music.setPitch(pitch);
    music.play();

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
                            pitch = 1.f;
                            music.setPitch(pitch);
                            continue;
                        }

                        sf::Vector2i cell = grid.cellAt(worldPos);

                        int selNumBefore = numbers.getSelectedNumber();



                        if (!(cell.x >= 0 && cell.y >= 0))
                            numbers.selectAtPosition(worldPos);
                        int selNum = (cell.x >= 0 && cell.y >= 0) ? selNumBefore : numbers.getSelectedNumber();


                        if (cell.x >= 0 && cell.y >= 0 && game.getCell(cell) != 0) {
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
                                int flatIdx = selectedCell.y * 9 + selectedCell.x;
                                if (solution[flatIdx] == selNum && game.isValidMove(selectedCell, selNum)) {
                                    game.setCell(selectedCell, selNum);
                                    pitch = 1.f;
                                    music.setPitch(pitch);
                                }
                                else {
                                    if (pitch >= 0.2f) {
                                        pitch -= 0.1f;
                                        music.setPitch(pitch);
                                    }

                                }

                            }
                        }
                        else
                        {
                            grid.selectCell({ -1, -1 });
                            if (!pencilMode && selNum != -1 && selectedCell.x >= 0 && selectedCell.y >= 0)
                            {
                                int flatIdx = selectedCell.y * 9 + selectedCell.x;
                                if (solution[flatIdx] == selNum && game.isValidMove(selectedCell, selNum)) {
                                    game.setCell(selectedCell, selNum);
                                    selectedCell = { -1, -1 };
                                    grid.selectCell(selectedCell);
                                }

                            }
                            selectedCell.x = -1;
                            selectedCell.y = -1;
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

                        if (!tools.isPencilActive() && selectedCell.x >= 0 && selectedCell.y >= 0)
                        {
                            int flatIdx = selectedCell.y * 9 + selectedCell.x;
                            if (solution[flatIdx] == num && game.isValidMove(selectedCell, num))
                            {
                                game.setCell(selectedCell, num);
                                selectedCell = { -1, -1 };
                                grid.selectCell(selectedCell);
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        grid.draw(window);

        drawBoardNumbers(window, game, grid, font, numbers.getSelectedNumber());

        if (game.isSolved() && okTexture.getSize().x > 0)
        {
            window.draw(okSprite);
        }

        numbers.draw(window);
        tools.draw(window);
        window.display();
    }
}

