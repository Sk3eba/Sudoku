#include "Numbers.h"
#include <algorithm>
#include <cmath>

Numbers::Numbers(sf::Vector2f position, float size, const sf::Font font)
    : m_position(position)
    , m_size(size)
    , m_cellSize(size / 3.f)
    , m_font(font)
{
    for (int i = 0; i < 9; ++i)
        m_numbers[i] = i + 1;

    rebuildShapes();
}

void Numbers::rebuildShapes()
{
    const float padding = std::max(4.f, m_cellSize * 0.08f);
    const float radius = std::max(1.f, (m_cellSize - 2.f * padding) * 0.5f);

    m_buttons.clear();
    m_texts.clear();
    m_buttons.reserve(9);
    m_texts.reserve(9);

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int idx = row * 3 + col;

            float cx = m_position.x + col * m_cellSize + m_cellSize * 0.5f;
            float cy = m_position.y + row * m_cellSize + m_cellSize * 0.5f;
			sf::Vector2f center(cx, cy);

            sf::CircleShape circle(radius);
			sf::Vector2f radiusVec(radius, radius);
            circle.setOrigin(radiusVec);
            circle.setPosition(center);
            circle.setFillColor(sf::Color(230, 230, 230));
            circle.setOutlineColor(sf::Color::Black);
            circle.setOutlineThickness(1.f);

            m_buttons.push_back(circle);

            sf::Text txt(m_font);
			const float size = std::max(8.f, radius * 1.1f);
            txt.setString(std::to_string(m_numbers[idx]));
            txt.setCharacterSize(size);
            txt.setFillColor(sf::Color::Black);
            sf::FloatRect tb = txt.getLocalBounds();
            txt.setOrigin(tb.getCenter());
            txt.setPosition(center);

            m_texts.push_back(txt);
        }
    }

    updateButtonVisuals();
    
}

void Numbers::updateButtonVisuals()
{
    for (size_t i = 0; i < m_buttons.size(); ++i)
    {
        if (static_cast<int>(i) == m_selectedIndex)
        {
            //selected appearance
            m_buttons[i].setFillColor(sf::Color(100, 180, 255)); // bluish
            m_buttons[i].setOutlineColor(sf::Color(30, 90, 160));
            m_buttons[i].setOutlineThickness(2.f);
        }
        else
        {
            //normal appearance
            m_buttons[i].setFillColor(sf::Color(230, 230, 230));
            m_buttons[i].setOutlineColor(sf::Color::Black);
            m_buttons[i].setOutlineThickness(1.f);
        }
    }
}

void Numbers::setPosition(sf::Vector2f position)
{
    m_position = position;
    rebuildShapes();
}

void Numbers::draw(sf::RenderTarget& target) const
{
    for (const auto& b : m_buttons)
        target.draw(b);

    for (const auto& t : m_texts)
        target.draw(t);
}

void Numbers::setSelectedCell(sf::Vector2i cell)
{
    if (cell.x < 0 || cell.x > 2 || cell.y < 0 || cell.y > 2)
    {
        m_selectedIndex = -1;
    }
    else
    {
        m_selectedIndex = cell.y * 3 + cell.x;
    }

    updateButtonVisuals();
}

sf::Vector2i Numbers::getSelectedCell() const
{
    if (m_selectedIndex < 0) return { -1, -1 };
    return { m_selectedIndex % 3, m_selectedIndex / 3 };
}

int Numbers::getSelectedNumber() const
{
    if (m_selectedIndex < 0) return -1;
    return m_numbers[m_selectedIndex];
}

void Numbers::selectAtPosition(sf::Vector2f worldPos)
{
    sf::Vector2i cell = cellAtPosition(worldPos);
    if (cell.x >= 0)
        setSelectedCell(cell);
    else
        setSelectedCell({ -1, -1 });
}

sf::Vector2i Numbers::cellAtPosition(sf::Vector2f worldPos) const
{
    const float padding = std::max(4.f, m_cellSize * 0.08f);
    const float radius = std::max(1.f, (m_cellSize - 2.f * padding) * 0.5f);

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int idx = row * 3 + col;
            float cx = m_position.x + col * m_cellSize + m_cellSize * 0.5f;
            float cy = m_position.y + row * m_cellSize + m_cellSize * 0.5f;

            float dx = worldPos.x - cx;
            float dy = worldPos.y - cy;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                return { col, row };
            }
        }
    }

    return { -1, -1 };
}