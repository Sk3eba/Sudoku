#include "Grid.h"

Grid::Grid(sf::Vector2f position, float size)
    : m_position(position)
    , m_size(size)
    , m_cellSize(size / 9.f)
{

    rebuildShapes();
}

void Grid::rebuildShapes()
{
    m_lines.clear();

    m_background.setSize({ m_size, m_size });
    m_background.setPosition(m_position);
    m_background.setFillColor(sf::Color::White);
    m_background.setOutlineColor(sf::Color::Black);
    m_background.setOutlineThickness(2.f);

    for (int i = 0; i <= 9; ++i)
    {
        const bool thick = (i % 3 == 0);
        const float thickness = thick ? 3.f : 1.f;

        // vertical
        sf::RectangleShape vline({ thickness, m_size });
        float vx = m_position.x + i * m_cellSize - thickness / 2.f;
        vline.setPosition({ vx, m_position.y });
        vline.setFillColor(sf::Color::Black);
        m_lines.push_back(vline);

        // horizontal
        sf::RectangleShape hline({ m_size, thickness });
        float hy = m_position.y + i * m_cellSize - thickness / 2.f;
        hline.setPosition({ m_position.x, hy });
        hline.setFillColor(sf::Color::Black);
        m_lines.push_back(hline);
    }
}

void Grid::draw(sf::RenderTarget &target) const
{
    target.draw(m_background);
    target.draw(highlightRect);
    for (const auto &line : m_lines)
        target.draw(line);
	
}

void Grid::setPosition(sf::Vector2f position)
{
    m_position = position;
    rebuildShapes();
}

sf::Vector2i Grid::cellAt(sf::Vector2f point){
    if (!m_background.getGlobalBounds().contains(point)) {
        highlightRect.setSize({ 0.f, 0.f });
        return { -1, -1 };
    }
		

    sf::Vector2f localPos = point - m_position;
	int col = static_cast<int>(localPos.x / m_cellSize);
	int row = static_cast<int>(localPos.y / m_cellSize);

	return sf::Vector2i(col, row);
}

sf::FloatRect Grid::getCellBounds(sf::Vector2i cell) const
{
    if (cell.x < 0 || cell.x > 8 || cell.y < 0 || cell.y > 8)
        return sf::FloatRect(); // empty

    sf::Vector2f localPos(m_position.x + cell.x * m_cellSize,
        m_position.y + cell.y * m_cellSize);

    sf::Vector2f size(m_cellSize, m_cellSize);

    return sf::FloatRect(
        localPos,
        size
    );
}

void Grid::selectCell(sf::Vector2i cell) {
    if (cell.x >= 0)
    {
        sf::FloatRect cellRect = getCellBounds(cell);
        highlightRect.setSize({ cellRect.size });
        highlightRect.setPosition({ cellRect.position });
        highlightRect.setFillColor(sf::Color(173, 216, 230, 160));
    }
}
