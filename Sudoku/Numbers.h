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
	void setNumber(sf::Vector2i cell, int number);
	int getNumber(sf::Vector2i cell) const;
private:
	void rebuildShapes();

	sf::Vector2f m_position;
	float m_size;
	float m_cellSize;
	const sf::Font font;

	std::array<int, 9> m_numbers;
	std::vector<sf::CircleShape> m_buttons;
	std::vector<sf::Text> m_texts;
};

