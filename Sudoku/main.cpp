#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Numbers.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Sudoku");
    window.setFramerateLimit(60);

    // create grid on the left
    Grid grid({ 20.f, 30.f }, 540.f);
	sf::Font font("Roboto-Regular.ttf");
    Numbers numbers({ 20.f + 540.f + 20.f, 30.f }, 205.f, font);

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
                        if (cell.x >= 0 && cell.y >= 0)
                        {
                            selectedCell = cell;
                            grid.selectCell(selectedCell);
                        }

                        numbers.selectAtPosition(worldPos);
                        if (cell.x >= 0) selectedCell = cell;
                    }
                }
                
            }
        }

        window.clear(sf::Color::White);

		grid.draw(window);
        numbers.draw(window);

        window.display();
    }
}