#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"


class Cube
{
private:
	float c_side = 100.0f;
	sf::Vector2f c_current_pos;
	sf::Vector2f c_new_pos;
	int c_cube_value;
	sf::RectangleShape c_cube;
	sf::Texture c_texture;
	sf::Text c_number;
	sf::Font c_font;
public:
	Cube() = default;
	Cube(int i);

	float get_size() const { return c_side; }
	int get_cube_value() const { return c_cube_value; }
	void set_position(float x, float y) { c_current_pos = { x,y }; }

	void draw(sf::RenderWindow& window);
	void change_pos(DIRECTION dir);

	void move_cube();
};

Cube::Cube(int i) {
	c_cube_value = i;

	if (i == 0) {
		c_cube.setSize(sf::Vector2f(0.0f, 0.0f));
	}
	else {
		c_cube.setSize(sf::Vector2f(c_side, c_side));
		if (!c_texture.loadFromFile("png/cube.png")) { exit(1); }
		c_cube.setTexture(&c_texture);

		if (!c_font.loadFromFile("font/troika.otf")) { exit(1); }
		c_number.setFont(c_font);
		c_number.setCharacterSize(70);
		i == 0 ? c_number.setString("") : c_number.setString(std::to_string(i));
		c_number.setFillColor(sf::Color(BEIGE));
		c_number.setOutlineThickness(2);
		c_number.setOutlineColor(sf::Color(DARK_BROWN));
	}
}

void Cube::draw(sf::RenderWindow& window) {
	c_cube.setPosition(c_current_pos);

	c_number.setPosition(sf::Vector2f(c_cube.getPosition().x + c_side / 2 - c_number.getGlobalBounds().width / 2,
									  c_cube.getPosition().y + c_side / 2 - c_number.getGlobalBounds().height));

	window.draw(c_cube);
	window.draw(c_number);
}


void Cube::change_pos(DIRECTION dir) {
	c_new_pos = c_current_pos;
	switch (dir) {
	case DIRECTION::UP:
		c_current_pos.x -= c_side;
		break;
	case DIRECTION::DOWN:
		c_current_pos.x += c_side;
		break;
	case DIRECTION::LEFT:
		c_current_pos.y -= c_side;
		break;
	case DIRECTION::RIGHT:
		c_current_pos.y += c_side;
		break;
	}

}
