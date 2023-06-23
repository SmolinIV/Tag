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
	float c_koef_smoothness;
	sf::RectangleShape c_cube;
	sf::Texture c_texture;
	sf::Text c_number;
	sf::Font c_font;
	DIRECTION c_dir;

public:
    Cube();
	Cube(int i);

	float get_size() const { return c_side; }
	int get_cube_value() const { return c_cube_value; }
	void set_position(float x, float y) { c_current_pos = { x,y }; }
	void draw(sf::RenderWindow& window);
	void change_pos(DIRECTION dir);
	void set_smoothness(float koef) { c_koef_smoothness = koef; }

	void moving_cube(bool& need_move);

	void reset(int i);
	~Cube() {}
};
