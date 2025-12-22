#include "Numbers.h"
#include <algorithm>
#include <cmath>

Numbers::Numbers(sf::Vector2f position, float size, const sf::Font font)
    : m_position(position)
    , m_size(size)
    , m_cellSize(size / 3.f)
{
    // initialize default numbers 1..9
    for (int i = 0; i < 9; ++i)
        m_numbers[i] = i + 1;

    rebuildShapes();
}

void Numbers::rebuildShapes()
{
    const float padding = std::max(4.f, m_cellSize * 0.08f);
    const float radius = std::max(1.f, (m_cellSize - 2.f * padding) * 0.5f);

    m_buttons.reserve(9);
    m_texts.reserve(9);

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int idx = row * 3 + col;

            // compute center for this button
            float cx = m_position.x + col * m_cellSize + m_cellSize * 0.5f;
            float cy = m_position.y + row * m_cellSize + m_cellSize * 0.5f;
			sf::Vector2f center(cx, cy);

            sf::CircleShape circle(radius);
			sf::Vector2f radiusVec(radius, radius);
            circle.setOrigin(radiusVec);
            circle.setPosition(center);
            circle.setFillColor(sf::Color(230, 230, 230)); // light gray button
            circle.setOutlineColor(sf::Color::Black);
            circle.setOutlineThickness(1.f);

            m_buttons.push_back(circle);

            sf::Text txt(font);
            txt.setString(std::to_string(m_numbers[idx]));
            // character size based on radius
            txt.setCharacterSize(static_cast<unsigned int>(std::max(8.f, radius * 1.1f)));
            txt.setFillColor(sf::Color::Black);
            // center text origin
            sf::FloatRect tb = txt.getLocalBounds();
            txt.setPosition(center);

            m_texts.push_back(txt);
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
    // draw buttons then text
    for (const auto& b : m_buttons)
        target.draw(b);

    // draw texts (only drawn correctly if font was set by caller)
    for (const auto& t : m_texts)
        target.draw(t);
}

void Numbers::setNumber(sf::Vector2i cell, int number)
{
    if (cell.x < 0 || cell.x > 2 || cell.y < 0 || cell.y > 2)
        return;
    int idx = cell.y * 3 + cell.x;
    m_numbers[idx] = number;

    // Update the corresponding text if present
    if (idx >= 0 && idx < static_cast<int>(m_texts.size()))
    {
        m_texts[idx].setString(std::to_string(number));
    }
}

int Numbers::getNumber(sf::Vector2i cell) const
{
    if (cell.x < 0 || cell.x > 2 || cell.y < 0 || cell.y > 2)
        return 0;
    int idx = cell.y * 3 + cell.x;
    return m_numbers[idx];
}