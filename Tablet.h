#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Tablet {

private:
	sf::RectangleShape t_tablet;
	sf::Vector2f t_tablet_size = { 400,70 };
	sf::Vector2f t_tablet_pos;
	sf::Texture t_tablet_texture;
	sf::Text t_tablet_text;
	sf::Font t_font;

	int t_text_size = 40;
public:
	Tablet(sf::String text, sf::Vector2f pos);

	void draw(sf::RenderWindow& window);

	void change_color(sf::Color color);

	sf::Vector2f get_pos() { return t_tablet_pos; }
	sf::Vector2f get_size() { return t_tablet_size; }
	void pressed();

};
