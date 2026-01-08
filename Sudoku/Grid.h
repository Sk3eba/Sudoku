#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Grid
{
public:
    Grid(sf::Vector2f position, float size);

    //draw grid to target
    void draw(sf::RenderTarget &target) const;

    //update grid top-left position
    void setPosition(sf::Vector2f position);

    sf::Vector2i cellAt(sf::Vector2f point);
    sf::FloatRect getCellBounds(sf::Vector2i cell) const;

	void selectCell(sf::Vector2i cell);
    
private:
    void rebuildShapes();

    sf::Vector2f m_position;
    float m_size;
    float m_cellSize;

    sf::RectangleShape m_background;
    std::vector<sf::RectangleShape> m_lines;
    sf::RectangleShape highlightRect;
};