#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class ToolButtons
{
public:
    // size - size of each square button (width == height)
    ToolButtons(sf::Vector2f position, float size, const sf::Font& font);

    void draw(sf::RenderTarget& target) const;
    void setPosition(sf::Vector2f position);

    // Handle a mouse click at worldPos. Internally toggles pencil or marks restart request.
    void selectAtPosition(sf::Vector2f worldPos);

    // Restart request is a one-shot; call to consumeRestartPressed() returns true once
    // and resets the internal flag.
    bool consumeRestartPressed();

    // Returns whether pencil (note) mode is active.
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