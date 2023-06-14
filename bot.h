#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constants.h"

class Bot {
private:
	Board b_board;
	float b_res_ftime;
	std::string b_res_strtime;
	int b_side_size;
	int b_moves;

	bool b_assemble_done;
	bool b_is_moving;
	bool b_cube_placed;
	int b_cube_value;
	struct Cube_position { 
		int i, j; 

		bool operator==(Cube_position& other) {
			return (this->i == other.i && this->j == other.j);
		}

	} cp_empty_pos, cp_req_cube_pos, cp_home_pos;

	void search_cubes_and_pos();

	enum class STAGE {
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		NONE
	} b_stage;


	void move_up_empty(sf::RenderWindow& window);
	void move_down_empty(sf::RenderWindow& window);
	void move_left_empty(sf::RenderWindow& window);
	void move_right_empty(sf::RenderWindow& window);

	void up_to_pos_req(sf::RenderWindow& window);
	void down_to_pos_req(sf::RenderWindow& window);
	void left_to_pos_req(sf::RenderWindow& window);
	void right_to_pos_req(sf::RenderWindow& window);

public:
	Bot();
	Bot(Board& board);

	float get_ftime_res() const { return b_res_ftime; }
	bool* get_permit_to_move() { return &b_is_moving; }
	const std::string& get_strtime_res() const { return b_res_strtime; }
	int get_moves() { return b_moves; }

	void reset_results();
	void set_fresult(float res) { b_res_ftime = res; };
	void set_strresult(std::string& res) { b_res_strtime = res; }
	void new_move() { ++b_moves; }
	Board& board() { return b_board; }
	bool assemble_done() { return b_assemble_done; }
	void set_start_pos();
	void assemble_board(sf::RenderWindow& window);
};

Bot::Bot() :
	b_res_ftime(0),
	b_res_strtime{ "00:00" },
	b_moves(0),
	b_side_size(0),
	b_assemble_done(false),
	b_is_moving(false),
	b_cube_placed(false),
	b_cube_value(0)
{}

Bot::Bot(Board& board) : 
	Bot() {
	b_board = board;
	b_stage = STAGE::NONE;
}

// Сброс метрик бота
void Bot::reset_results() {
	b_res_ftime = 0;
	b_res_strtime = { "00:00" };
	b_moves = 0;
}

// Задание "точки старта" для сборки - поиск кубика с цифрой 1, определение его должной позиции (далее - "дом") пустого пространства 
void Bot::set_start_pos() {
	b_side_size = b_board.get_board_side_size();
	for (int i = 0; i < b_side_size; i++) {
		for (int j = 0; j < b_side_size; j++) {
			if (b_board(i, j) == 1) { 
				cp_req_cube_pos = { i,j };
			}
			if (b_board(i, j) == 0) {
				cp_empty_pos = { i,j };
			}
		}
	}
	cp_home_pos = { 0,0 };
	b_cube_value = 1;
}

// Поиск следующего кубика и обновление данных "дома"
void Bot::search_cubes_and_pos() {
	if (cp_home_pos.j == b_side_size - 1) {
		++cp_home_pos.i;
		cp_home_pos.j = 0;
	}
	else {
		++cp_home_pos.j;
	}

	for (int i = 0; i < b_side_size; i++) {
		for (int j = 0; j < b_side_size; j++) {
			if (b_board(i, j) == b_cube_value) {
				cp_req_cube_pos = { i,j };
				return;
			}
		}
	}
}


void Bot::assemble_board(sf::RenderWindow& window) {
	
	//if (b_cube_value == 4) { return; }

	if (b_is_moving) { return; }
	/*
	if (b_stage == STAGE::NONE); {
		if (b_assemble_done) { return; }

		if (!b_is_moving) {
			while (true) {
				if (cp_req_cube_pos == cp_home_pos) {
					b_cube_placed = true;
				}
				else {
					break;
				}

				if (b_cube_placed) {
					++b_cube_value;
					search_cubes_and_pos();
					b_cube_placed = false;
				}
			}
		}*/
		if (cp_empty_pos.i <= cp_home_pos.i) {
			move_down_empty(window);
			return;
		} 
		if (cp_empty_pos.i > cp_home_pos.i) {
			move_up_empty(window);
			return;
		}
	
}

void Bot::move_up_empty(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i - 1, cp_empty_pos.j })) {
		cp_empty_pos.i -= 1;
		b_is_moving = true;
		return;
	}
}

void Bot::move_down_empty(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i + 1, cp_empty_pos.j })) {
		cp_empty_pos.i += 1;
		b_is_moving = true;
		return;
	}
}

void Bot::move_left_empty(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i, cp_empty_pos.j - 1 })) {
		cp_empty_pos.j -= 1;
		b_is_moving = true;
		return;
	}
}
void Bot::move_right_empty(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i, cp_empty_pos.j + 1 })) {
		cp_empty_pos.j += 1;
		b_is_moving = true;
		return;
	}
}