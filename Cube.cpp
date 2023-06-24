#include "Cube.h"

Cube::Cube() {
	c_cube_value = 0;
	c_koef_smoothness = 20;
	c_current_pos = { 0,0 };
	c_new_pos = { 0,0 };
	c_cube.setSize(sf::Vector2f(0.0f, 0.0f));
	if (!c_texture.loadFromFile("png/cube.png")) { exit(1); }
	c_cube.setTexture(&c_texture);
	if (!c_font.loadFromFile("font/troika.otf")) { exit(1); }
	c_number.setString("");
	c_number.setFont(c_font);
	c_number.setCharacterSize(70);
	c_number.setFillColor(sf::Color(RGB_BEIGE));
	c_number.setOutlineThickness(2);
	c_number.setOutlineColor(sf::Color(RGB_DARK_BROWN));
	c_dir = DIRECTION::NON_DIR;
}

Cube::Cube(int i) : Cube() {
	if (i != 0) {
		c_cube_value = i;
		c_cube.setSize(sf::Vector2f(c_side, c_side));
		c_number.setString(std::to_string(i));
	}
}

void Cube::draw(sf::RenderWindow& window) {
	c_cube.setPosition(c_current_pos);
	
	c_number.setPosition(sf::Vector2f(c_current_pos.x + c_side / 2 - c_number.getGlobalBounds().width / 2,
		c_current_pos.y + c_side / 2 - c_number.getGlobalBounds().height));

	window.draw(c_cube);
	window.draw(c_number);
}

void Cube::change_pos(DIRECTION dir) {
	c_new_pos = c_current_pos;
	c_dir = dir;
	switch (dir) {
	case DIRECTION::UP:
		c_new_pos.y -= c_side;
		break;
	case DIRECTION::DOWN:
		c_new_pos.y += c_side;
		break;
	case DIRECTION::LEFT:
		c_new_pos.x -= c_side;
		break;
	case DIRECTION::RIGHT:
		c_new_pos.x += c_side;
		break;
	}
	if (c_cube_value == 0) { // ѕустое пространство не имет текстуры, поэтому сразу без анимации занимает нужное поолжение
		c_current_pos = c_new_pos;
		c_dir = DIRECTION::NON_DIR;
	}

}

void Cube::moving_cube(bool& need_move) { 
	
	switch (c_dir) {
	case DIRECTION::UP:
		if (c_current_pos.y > c_new_pos.y) {
			c_current_pos.y -= c_koef_smoothness;
		}
		else {
			c_current_pos = c_new_pos;
			need_move = false;
		}
		break;
	case DIRECTION::DOWN:
		if (c_current_pos.y < c_new_pos.y) {
			c_current_pos.y += c_koef_smoothness;
		}
		else {
			c_current_pos = c_new_pos;
			need_move = false;
		}
		break;
	case DIRECTION::LEFT:
		if (c_current_pos.x > c_new_pos.x) {
			c_current_pos.x -= c_koef_smoothness;
		}
		else {
			c_current_pos = c_new_pos;
			need_move = false;
		}
		break;
	case DIRECTION::RIGHT:
		if (c_current_pos.x < c_new_pos.x) {
			c_current_pos.x += c_koef_smoothness;
		}
		else {
			c_current_pos = c_new_pos;
			need_move = false;
		}
		break;
	defualt:
		need_move = false;
		c_dir = DIRECTION::NON_DIR;
	}
}

void Cube::reset(int i) {
	if (i != 0) {
		c_cube_value = i;
		c_number.setString(std::to_string(i));
	}
}