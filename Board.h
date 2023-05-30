#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Cube.h"
#include "Constants.h"

class Board
{
private:
	float b_vert_bord = 15.0f;
	float b_horiz_bord = 15.0f;
	float b_width = 400.0f + 2 * b_horiz_bord;
	float b_height = 400.0f + 2 * b_vert_bord;
	sf::RectangleShape b_board;
	sf::Texture b_texture;
	Cube* b_moving_cube;	
	bool b_already_drawn;
	static const int b_board_capacity = 4;
	std::array<std::array<Cube, b_board_capacity>, b_board_capacity> b_cubes{{{1, 2, 3, 4}, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, {13, 14, 15, 0}}};
	short b_empty_cube_i = 3;
	short b_empty_cube_j = 3;
public:
	Board();
	float get_board_width() const { return b_width; }
	float get_board_height() const { return b_height; }
	void draw(sf::RenderWindow& window, float xpos, float ypos, bool &need_move);
	void shaffle_board(sf::RenderWindow& window);
	bool ask_for_moving(sf::RenderWindow& window, sf::Vector2i click_pos, bool mouse_control = false);
	void swap_cubes(int i1, int j1, int i2, int j2 );
};


Board::Board() {
	b_board.setSize(sf::Vector2f(b_width, b_height));
	if (!b_texture.loadFromFile("png/deck.png")) { exit(0); };
	b_board.setTexture(&b_texture);
	b_already_drawn = false;
}

void Board::draw(sf::RenderWindow& window, float xpos, float ypos, bool& need_move) {
	if (!b_already_drawn) {
		b_board.setPosition(sf::Vector2f(xpos - b_board.getGlobalBounds().width / 2.0f, ypos - b_board.getGlobalBounds().height / 2.0f));
		window.draw(b_board);
		float cube_size = b_cubes[0][0].get_size();
		for (int i = 0; i < b_board_capacity; i++) {
			for (int j = 0; j < b_board_capacity; j++) {
				b_cubes[i][j].set_position(b_board.getPosition().x + b_horiz_bord + cube_size * j, b_board.getPosition().y + b_vert_bord + cube_size * i);
				b_cubes[i][j].draw(window);
			}
		}
		b_already_drawn = true;
	}
	else {
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
}

bool Board::ask_for_moving(sf::RenderWindow& window, sf::Vector2i target_pos, bool mouse_control) {
	
	int i, j;

	if (mouse_control) {
		i = (target_pos.y - b_board.getPosition().y - b_vert_bord);
		j = (target_pos.x - b_board.getPosition().x - b_horiz_bord);
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
			return true;
		}
	}

	if (i < b_board_capacity - 1) {
		if (b_cubes[i + 1][j].get_cube_value() == 0) {
			swap_cubes(i + 1, j, i, j);
			b_cubes[i][j].change_pos(DIRECTION::UP);
			b_cubes[i + 1][j].change_pos(DIRECTION::DOWN);
			b_moving_cube = &b_cubes[i + 1][j];
			return true;
		}
	}

	if (j > 0) {
		if (b_cubes[i][j - 1].get_cube_value() == 0) {
			swap_cubes(i, j - 1, i, j);
			b_cubes[i][j].change_pos(DIRECTION::RIGHT);
			b_cubes[i][j - 1].change_pos(DIRECTION::LEFT);
			b_moving_cube = &b_cubes[i][j - 1];
			return true;
		}
	}

	if (j < b_board_capacity - 1) {
		if (b_cubes[i][j + 1].get_cube_value() == 0) {
			swap_cubes(i, j + 1, i, j);
			b_cubes[i][j].change_pos(DIRECTION::LEFT);
			b_cubes[i][j + 1].change_pos(DIRECTION::RIGHT);
			b_moving_cube = &b_cubes[i][j + 1];
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

	int	nexti = b_empty_cube_i,	 // —генерированные индексы €чейки дл€ swap'а с текущей
		nextj = b_empty_cube_j,	 //
		lasti = b_empty_cube_i,	 // »ндексы предыдущей €чейки (при успешном swap'е)
		lastj = b_empty_cube_j,     //
		unit = 0;		 // рандомна€ плюс или минус единица 

	while (true) {
		unit = (rand() % 2) ? (1) : (-1);
		unit = (rand() % 2) ? (1) : (-1);
		(rand() % 2) ? (nexti = b_empty_cube_i + unit) : (nextj = b_empty_cube_j + unit);

		if (!ask_for_moving(window, sf::Vector2i(nexti,nextj), false)) {
			nexti = b_empty_cube_i;
			nextj = b_empty_cube_j;
		}
		else {
			break;
		}
	}

	lasti = b_empty_cube_i;
	lastj = b_empty_cube_j;
	b_empty_cube_i = nexti;
	b_empty_cube_j = nextj;
}