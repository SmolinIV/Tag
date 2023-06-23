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
	static const int b_side_size = 4;
	std::array<std::array<Cube, b_side_size>, b_side_size> b_cubes;
	struct cube_pos { int i, j; } b_empty_cube_pos, b_last_swap_cube;

	void set_cubes_position();
public:
	Board();
	Board(std::error_code& ec);
	float get_board_width() const { return b_width; }
	float get_board_height() const { return b_height; }
	int get_board_side_size() { return b_side_size; }
	void setPosition(float xpos, float ypos);
	void set_smoothness(float koef);
	void draw(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, bool &need_move);

	void shaffle_board(sf::RenderWindow& window);
	bool ask_for_moving(sf::RenderWindow& window, sf::Vector2i click_pos, bool mouse_control = false);
	void swap_cubes(int i1, int j1, int i2, int j2 );
	bool sequence_restored();
	std::array<Cube, b_side_size>& operator[](unsigned int index) { return b_cubes[index]; }
	int operator()(unsigned int i, unsigned int j) { return b_cubes[i][j].get_cube_value(); } // Проще в реализации и эффективнее с точ.зр. производителдьности
	//Board& operator=(Board& other);
	void reset();

	~Board() {}

};