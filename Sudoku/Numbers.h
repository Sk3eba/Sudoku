#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

class Numbers
{
public:
	Numbers(sf::Vector2f position, float size, const sf::Font font);
	// Draw numbers to any sf::RenderTarget (window or render texture)
	void draw(sf::RenderTarget &target) const;
	// Update numbers top-left position (recomputes shapes)
	void setPosition(sf::Vector2f position);

	// selection API
	void setSelectedCell(sf::Vector2i cell);            // select a cell (use {-1,-1} to clear)
	sf::Vector2i getSelectedCell() const;
	int getSelectedNumber() const;                       // -1 if none selected
	// convenience: select by world position (e.g., mouse)
	void selectAtPosition(sf::Vector2f worldPos);
private:
	void rebuildShapes();
	void updateButtonVisuals();
	sf::Vector2i cellAtPosition(sf::Vector2f worldPos) const;

	sf::Vector2f m_position;
	float m_size;
	float m_cellSize;
	const sf::Font m_font;

	std::array<int, 9> m_numbers;
	std::vector<sf::CircleShape> m_buttons;
	std::vector<sf::Text> m_texts;

	int m_selectedIndex = -1;
};

