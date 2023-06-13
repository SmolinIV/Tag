#pragma once
#include <SFML/Graphics.hpp>
#include <system_error>
#include <array>
#include "Cube.h"
#include "Constants.h"


class Board
{
private:
	float b_vert_bord;
	float b_horiz_bord;
	float b_width;
	float b_height;
	sf::RectangleShape b_board;
	sf::Texture b_texture;
	sf::Vector2f b_board_pos;
	Cube* b_moving_cube;	
	static const int b_board_capacity = 4;
	std::array<std::array<Cube, b_board_capacity>, b_board_capacity> b_cubes;
	struct cube_pos { int i, j; } b_empty_cube, b_last_swap_cube;

	void set_cubes_position();
public:
	Board();
	Board(std::error_code& ec);
	float get_board_width() const { return b_width; }
	float get_board_height() const { return b_height; }
	void draw(sf::RenderWindow& window, bool &need_move);
	void shaffle_board(sf::RenderWindow& window);
	bool ask_for_moving(sf::RenderWindow& window, sf::Vector2i click_pos, bool mouse_control = false);
	void swap_cubes(int i1, int j1, int i2, int j2 );
	bool sequence_restored();
	void setPosition(float xpos, float ypos);
	void reset();
	~Board() {}

};

Board::Board() : b_vert_bord(15.0f),
b_horiz_bord(15.0f),
b_width(400.0f + 2 * b_horiz_bord),
b_height(400.0f + 2 * b_vert_bord),
b_board_pos{ 0,0 },
b_cubes{ {{1, 2, 3, 4}, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, {13, 14, 15, 0}} },
b_empty_cube{ 3,3 },
b_last_swap_cube{ 3,3 }
{}

Board::Board(std::error_code& ec) : Board() {
	do {
		b_board.setSize(sf::Vector2f(b_width, b_height));
		if (!b_texture.loadFromFile("png/deck.png")) { ec.default_error_condition(); break; };
		b_board.setTexture(&b_texture);
	} while (false);
}

void Board::setPosition (float xpos, float ypos) {
	b_board_pos = { xpos - b_board.getGlobalBounds().width / 2.0f, ypos - b_board.getGlobalBounds().height / 2.0f };
	b_board.setPosition(b_board_pos);
	set_cubes_position();
}

void Board::set_cubes_position() {
	float cube_size = b_cubes[0][0].get_size();
	for (int i = 0; i < b_board_capacity; i++) {
		for (int j = 0; j < b_board_capacity; j++) {
			b_cubes[i][j].set_position(b_board_pos.x + b_horiz_bord + cube_size * j, b_board_pos.y + b_vert_bord + cube_size * i);
		}
	}
}

void Board::draw(sf::RenderWindow& window, bool& need_move) {
	window.draw(b_board);
	float cube_size = b_cubes[0][0].get_size();
	for (int i = 0; i < b_board_capacity; i++) {
		for (int j = 0; j < b_board_capacity; j++) {
			//b_cubes[i][j].set_position(b_board_pos.x + b_horiz_bord + cube_size * j, b_board_pos.y + b_vert_bord + cube_size * i);
			b_cubes[i][j].draw(window);
		}
	}

	window.draw(b_board);
	if (need_move) {
		b_moving_cube->moving_cube(need_move);
	}
	for (int i = 0; i < b_board_capacity; i++) {
		for (int j = 0; j < b_board_capacity; j++) {
			b_cubes[i][j].draw(window);
		}
	}

}

bool Board::ask_for_moving(sf::RenderWindow& window, sf::Vector2i target_pos, bool mouse_control) {

	int i, j;

	if (mouse_control) {
		i = (target_pos.y - b_board_pos.y - b_vert_bord);
		j = (target_pos.x - b_board_pos.x - b_horiz_bord);
		if (i < 0 || j < 0) { return false; }
		i /= b_cubes[0][0].get_size();
		j /= b_cubes[0][0].get_size();
	}
	else {
		i = target_pos.y;
		j = target_pos.x;
		if (i < 0 || j < 0) { return false; }
	}
	if (i >= b_board_capacity || j >= b_board_capacity) { return false; }

	if (i > 0) {
		if (b_cubes[i - 1][j].get_cube_value() == 0) {
			swap_cubes(i - 1, j, i, j);
			b_cubes[i][j].change_pos(DIRECTION::DOWN);
			b_cubes[i - 1][j].change_pos(DIRECTION::UP);
			b_moving_cube = &b_cubes[i - 1][j];
			b_empty_cube = { i,j };
			b_last_swap_cube = { i - 1,j };
			return true;
		}
	}

	if (i < b_board_capacity - 1) {
		if (b_cubes[i + 1][j].get_cube_value() == 0) {
			swap_cubes(i + 1, j, i, j);
			b_cubes[i][j].change_pos(DIRECTION::UP);
			b_cubes[i + 1][j].change_pos(DIRECTION::DOWN);
			b_moving_cube = &b_cubes[i + 1][j];
			b_empty_cube = { i,j };
			b_last_swap_cube = { i + 1,j };
			return true;
		}
	}

	if (j > 0) {
		if (b_cubes[i][j - 1].get_cube_value() == 0) {
			swap_cubes(i, j - 1, i, j);
			b_cubes[i][j].change_pos(DIRECTION::RIGHT);
			b_cubes[i][j - 1].change_pos(DIRECTION::LEFT);
			b_moving_cube = &b_cubes[i][j - 1];
			b_empty_cube = { i,j };
			b_last_swap_cube = { i,j - 1 };
			return true;
		}
	}

	if (j < b_board_capacity - 1) {
		if (b_cubes[i][j + 1].get_cube_value() == 0) {
			swap_cubes(i, j + 1, i, j);
			b_cubes[i][j].change_pos(DIRECTION::LEFT);
			b_cubes[i][j + 1].change_pos(DIRECTION::RIGHT);
			b_moving_cube = &b_cubes[i][j + 1];
			b_empty_cube = { i,j };
			b_last_swap_cube = { i,j + 1 };
			return true;
		}
	}

	return false;
}


void Board::swap_cubes(int i1, int j1, int i2, int j2) {
	Cube temp_cube = b_cubes[i1][j1];
	b_cubes[i1][j1] = b_cubes[i2][j2];
	b_cubes[i2][j2] = temp_cube;
}


void Board::shaffle_board(sf::RenderWindow& window) {

	int next_i,
		next_j,
		unit;

	while (true) {
		next_i = b_empty_cube.i;
		next_j = b_empty_cube.j;
		unit = (rand() % 2) ? (1) : (-1);
		(rand() % 2) ? (next_i = b_empty_cube.i + unit) : (next_j = b_empty_cube.j + unit);

		if (!(next_i == b_last_swap_cube.i && next_j == b_last_swap_cube.j)) {
			if (ask_for_moving(window, sf::Vector2i(next_j, next_i))) {
				break;
			}
		}
	}
}

bool Board::sequence_restored() {
	if (!(b_cubes[3][2].get_cube_value() == 15 && b_cubes[3][3].get_cube_value() == 0)) { return false; }

	int cube_value = 1;
	for (int i = 0; i < b_board_capacity; i++) {
		for (int j = 0; j < b_board_capacity; j++) {
			if (i == b_board_capacity - 1 && j == b_board_capacity - 1) { return true; }
			if (!(b_cubes[i][j].get_cube_value() == cube_value)) { return false; }
			++cube_value;
		}
	}

}

void Board::reset() {
	swap_cubes(b_empty_cube.i, b_empty_cube.j, b_board_capacity - 1, b_board_capacity - 1);
	int number = 1;

	for (int i = 0; i < b_board_capacity; i++) {
		for (int j = 0; j < b_board_capacity; j++) {
			if (b_cubes[i][j].get_cube_value() == 0) { break; }
			b_cubes[i][j].reset(number);
				++number;
		}
	}

	set_cubes_position();

	b_empty_cube = { 3,3 }; 
	b_last_swap_cube = { 3,3 };
}