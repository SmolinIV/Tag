#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constants.h"
#include "Board.h"

class Bot {
private:
	Board b_board;
	float b_res_ftime;
	int b_side_size;
	int b_game_moves;
	struct AUXILIARY_VARIABLES {
		int method_step_num;
		int stage_step;
		int little_circle_quadrant;
		bool clockwise;
		bool start_pos_at_the_top;
	} b_av;

	bool b_assemble_done;
	bool b_is_moving;
	bool b_cube_placed;
	int b_req_cube_value;
	struct Cube_position { 
		int i, j; 

		bool operator==(Cube_position& other) {
			return (this->i == other.i && this->j == other.j);
		}

		bool operator!=(Cube_position& other) {
			return !(this->i == other.i && this->j == other.j);
		}

	} cp_empty_pos, cp_req_cube_pos, cp_home_pos, cp_temp_pos;


	enum class STAGE {
		ASSEMBLE_ROW,
		PLACED_LAST_CUBE,
		ASSEMBLE_LAST_ROWS,
		NONE
	} b_stage;

	enum class MOVING_METHOD {
		EMPTY_UNDER_REQ,
		EMPTY_ON_RIGHT_FROM_REQ,
		REQ_UP_TO_POS,
		REQ_LEFT_TO_POS,
		REQ_RIGHT_TO_POS,
		LITTLE_CIRCLE,
		MIDDLE_CIRCLE,
		NONE
	} b_moving_method;


	void search_cubes_and_pos();
	void search_certain_cube_and_pos();

	void move_empty_up(sf::RenderWindow& window);
	void move_empty_down(sf::RenderWindow& window);
	void move_empty_left(sf::RenderWindow& window);
	void move_empty_right(sf::RenderWindow& window);
	void placed_empty_under_req(sf::RenderWindow& window);
	void place_empty_on_right_of_req(sf::RenderWindow& window);

	void move_req_up_to_pos(sf::RenderWindow& window);
	void move_req_left_to_pos(sf::RenderWindow& window);
	void move_req_right_to_pos(sf::RenderWindow& window);
	
	void assemble_row_exept_last_cube(sf::RenderWindow& window);
	void assemble_last_two_rows(sf::RenderWindow& window);

	void placed_last_cube(sf::RenderWindow& window);
	void middle_circle_movement(sf::RenderWindow& window);
	void little_circle_movement(sf::RenderWindow& window);

	bool move_cube_to_given_pos(sf::RenderWindow& window, Cube_position& given_pos);

public:
	Bot();
	Bot(Board& board);

	float get_ftime_res() const { return b_res_ftime; }
	bool* get_permit_to_move() { return &b_is_moving; }
	int get_moves() { return b_game_moves; }

	void reset_results();
	void set_ftime_res(float res) { b_res_ftime = res; };
	void new_move() { ++b_game_moves; }
	Board& board() { return b_board; }
	bool assemble_done() { return b_assemble_done; }
	void set_start_pos();
	void assemble_board(sf::RenderWindow& window);
};
