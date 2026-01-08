#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class ToolButtons
{
public:
    ToolButtons(sf::Vector2f position, float size, const sf::Font& font);

    void draw(sf::RenderTarget& target) const;
    void setPosition(sf::Vector2f position);

    void selectAtPosition(sf::Vector2f worldPos);

    bool restartPressed();

    bool isPencilActive() const;

private:
    void rebuildShapes();
    void updateVisuals();

    sf::Vector2f m_position;
    float m_size;
    const sf::Font& m_font;

    std::vector<sf::RectangleShape> m_buttons; // 0 = restart, 1 = pencil
    std::vector<sf::Text> m_texts;

    bool m_pencilActive = false;
    bool m_restartPressed = false;
};