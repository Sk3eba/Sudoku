#include "ToolButtons.h"

ToolButtons::ToolButtons(sf::Vector2f position, float size, const sf::Font & font)
    : m_position(position)
    , m_size(size)
    , m_font(font)
{
    rebuildShapes();
    updateVisuals();
}

void ToolButtons::rebuildShapes()
{
    const float gap = m_size * 0.2f;

    m_buttons.clear();
    m_texts.clear();
    m_buttons.reserve(2);
    m_texts.reserve(2);

    for (int i = 0; i < 2; ++i)
    {
        float offsetX = static_cast<float>(i) * (m_size + gap);
        sf::Vector2f center(m_position.x + offsetX + m_size * 0.5f, m_position.y + m_size * 0.5f);

        sf::RectangleShape rect({ m_size, m_size });
        rect.setOrigin(sf::Vector2f(m_size * 0.5f, m_size * 0.5f));
        rect.setPosition(center);
        rect.setOutlineThickness(2.f);
        rect.setOutlineColor(sf::Color(100, 100, 100));
        rect.setFillColor(sf::Color(230, 230, 230));

        m_buttons.push_back(rect);

        sf::Text txt(m_font);
        txt.setString(i == 0 ? "R" : "P");
        unsigned int charSize = static_cast<unsigned int>(m_size * 0.55f);
        txt.setCharacterSize(charSize);
        txt.setFillColor(sf::Color::Black);

        sf::FloatRect tb = txt.getLocalBounds();
        txt.setOrigin(tb.getCenter());
        txt.setPosition(center);

        m_texts.push_back(txt);
    }

    updateVisuals();
}

void ToolButtons::updateVisuals()
{
    if (m_buttons.size() < 2) return;

    if (m_pencilActive)
    {
        m_buttons[1].setFillColor(sf::Color(200, 220, 255));
        m_buttons[1].setOutlineColor(sf::Color(50, 120, 200));
    }
    else
    {
        m_buttons[1].setFillColor(sf::Color(230, 230, 230));
        m_buttons[1].setOutlineColor(sf::Color(100, 100, 100));
    }
}

void ToolButtons::draw(sf::RenderTarget& target) const
{
    for (const auto& b : m_buttons) target.draw(b);
    for (const auto& t : m_texts) target.draw(t);
}

void ToolButtons::setPosition(sf::Vector2f position)
{
    m_position = position;
    rebuildShapes();
}

void ToolButtons::selectAtPosition(sf::Vector2f worldPos)
{
    if (!m_buttons.empty() && m_buttons[0].getGlobalBounds().contains(worldPos))
    {
        m_restartPressed = true;
        return;
    }

    if (m_buttons.size() >= 2 && m_buttons[1].getGlobalBounds().contains(worldPos))
    {
        m_pencilActive = !m_pencilActive;
        updateVisuals();
    }
}

bool ToolButtons::restartPressed()
{
    if (m_restartPressed)
    {
        m_restartPressed = false;
        return true;
    }
    return false;
}

bool ToolButtons::isPencilActive() const
{
    return m_pencilActive;
}