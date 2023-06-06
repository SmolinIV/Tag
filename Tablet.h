#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Tablet {

private:
	sf::RectangleShape t_tablet;
	sf::Vector2f t_tablet_size = { 400,60 };
	sf::Vector2f t_tablet_pos;
	sf::Texture t_tablet_texture;
	sf::Text t_tablet_text;
	sf::Font t_font;

	int t_text_size = 40;
public:
	Tablet(sf::String text, sf::Vector2f pos);

	void draw(sf::RenderWindow& window);

	void change_color(sf::Color color);

	void pressed();

};

Tablet::Tablet(sf::String text, sf::Vector2f pos) {

	t_tablet.setSize(t_tablet_size);
	if (!t_tablet_texture.loadFromFile("png/tablet3.png")) { exit(1); }
	t_tablet.setTexture(&t_tablet_texture);
	t_tablet_pos.x = pos.x - t_tablet_size.x/2;
	t_tablet_pos.y = pos.y - t_tablet_size.y / 2;

	if (!t_font.loadFromFile("font/Oldscriptc.otf")) { exit(1); }
	t_tablet_text.setString(text);
	t_tablet_text.setFont(t_font);
	t_tablet_text.setCharacterSize(t_text_size);
	t_tablet_text.setFillColor(sf::Color(BEIGE));
	t_tablet_text.setOutlineThickness(2);
	t_tablet_text.setOutlineColor(sf::Color(DARK_BROWN));
}

void Tablet::draw(sf::RenderWindow& window) {
	t_tablet.setPosition(t_tablet_pos);
	t_tablet_text.setPosition(t_tablet_pos.x + t_tablet_size.x / 2 - t_tablet_text.getGlobalBounds().width / 2,
						      t_tablet_pos.y + t_tablet_size.y / 2 - t_tablet_text.getGlobalBounds().height + 10);

	window.draw(t_tablet);
	window.draw(t_tablet_text);

}

void Tablet::change_color(sf::Color color) {
	t_tablet_text.setFillColor(color);
}


