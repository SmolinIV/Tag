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
	const std::string& get_strtime_res() const { return b_res_strtime; }
	int get_moves() { return b_game_moves; }

	void reset_results();
	void set_fresult(float res) { b_res_ftime = res; };
	void set_strresult(std::string& res) { b_res_strtime = res; }
	void new_move() { ++b_game_moves; }
	Board& board() { return b_board; }
	bool assemble_done() { return b_assemble_done; }
	void set_start_pos();
	void assemble_board(sf::RenderWindow& window);
};

Bot::Bot() :
	b_res_ftime(0),
	b_res_strtime{ "00:00" },
	b_game_moves(0),
	b_side_size(0),
	b_assemble_done(false),
	b_is_moving(false),
	b_cube_placed(false),
	b_req_cube_value(0),
	b_moving_method(MOVING_METHOD::NONE),
	b_stage(STAGE::NONE),
	b_av{0, 0, 0, false, false}
{}

Bot::Bot(Board& board) : 
	Bot() {
	b_board = board; // Т.к. как нет работы с дин. памятью, компилятор и сам создаст отличный конструктор копирования
}

// Сброс метрик бота
void Bot::reset_results() {
	b_res_ftime = 0;
	b_res_strtime = { "00:00" };
	b_game_moves = 0;
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
	b_req_cube_value = 1;
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
			if (b_board(i, j) == b_req_cube_value) {
				cp_req_cube_pos = { i,j };
				return;
			}
		}
	}

	b_assemble_done = true;
}


void Bot::assemble_board(sf::RenderWindow& window) {
	
	if (b_assemble_done || b_is_moving) { return; }

	switch (b_moving_method)
	{
	case MOVING_METHOD::NONE:
		break;
	case MOVING_METHOD::REQ_UP_TO_POS:
		move_req_up_to_pos(window);
		return;
	case MOVING_METHOD::REQ_LEFT_TO_POS:
		move_req_left_to_pos(window);
		return;
	case MOVING_METHOD::REQ_RIGHT_TO_POS:
		move_req_right_to_pos(window);
		return;
	case MOVING_METHOD::EMPTY_UNDER_REQ:
		placed_empty_under_req(window);
		return;
	case MOVING_METHOD::LITTLE_CIRCLE:
		little_circle_movement(window);
		return;
	case MOVING_METHOD::MIDDLE_CIRCLE:
		middle_circle_movement(window);
		return;
	default:
		break;
	}

	switch (b_stage)
	{
	case STAGE::NONE:
		break;
	case STAGE::ASSEMBLE_ROW:
		assemble_row_exept_last_cube(window);
		return;
	case STAGE::PLACED_LAST_CUBE:
		placed_last_cube(window);
		return;
	case STAGE::ASSEMBLE_LAST_ROWS:
		assemble_last_two_rows(window);
		return;
	default:
		break;
	}

	while (true) {
		if (cp_req_cube_pos == cp_home_pos) {
			b_cube_placed = true;
		}
		else {
			break;
		}

		if (b_cube_placed) {
			++b_req_cube_value;
			search_cubes_and_pos();
			b_cube_placed = false;
			b_av.stage_step = 1;
		}
		if (b_assemble_done) { return; }
	}  
	

	// Сборка всех рядов кроме двух последних
	if (cp_home_pos.i < b_side_size - 2) {
		if (cp_home_pos.j < b_side_size - 1) {
			b_stage = STAGE::ASSEMBLE_ROW;
			assemble_row_exept_last_cube(window);
		}
		else {
			//Случай, когда пустота справа и под ней нужное число
			if (cp_empty_pos == cp_home_pos) {
				move_empty_down(window);
			}
			else {
				cp_temp_pos = cp_home_pos;
				cp_home_pos.i += 1;
				b_stage = STAGE::PLACED_LAST_CUBE;
				placed_last_cube(window);
			}
		}
	}
	else {
		b_av.stage_step = 1;
		b_stage = STAGE::ASSEMBLE_LAST_ROWS;
		assemble_last_two_rows(window);
	}
}

void Bot::move_empty_up(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i - 1, cp_empty_pos.j })) {
		--cp_empty_pos.i;
		if (cp_empty_pos == cp_req_cube_pos) {
			++cp_req_cube_pos.i;
		}
		b_is_moving = true;
	}
}
void Bot::move_empty_down(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i + 1, cp_empty_pos.j })) {
		++cp_empty_pos.i;
		if (cp_empty_pos == cp_req_cube_pos) {
			--cp_req_cube_pos.i;
		}
		b_is_moving = true;
	}
}
void Bot::move_empty_left(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i, cp_empty_pos.j - 1 })) {
		--cp_empty_pos.j;
		if (cp_empty_pos == cp_req_cube_pos) {
			++cp_req_cube_pos.j;
		}
		b_is_moving = true;
	}
}
void Bot::move_empty_right(sf::RenderWindow& window) {
	if (b_board.ask_for_moving(window, { cp_empty_pos.i, cp_empty_pos.j + 1 })) {
		cp_empty_pos.j += 1;
		if (cp_empty_pos == cp_req_cube_pos) {
			--cp_req_cube_pos.j;
		}
		b_is_moving = true;
	}
}
void Bot::move_req_left_to_pos(sf::RenderWindow& window) {

	if (cp_empty_pos.j < cp_req_cube_pos.j) {
		if (cp_empty_pos.i == cp_req_cube_pos.i) {
			move_empty_right(window);
		}
		else {
			if (cp_empty_pos.i < cp_req_cube_pos.i) {
				move_empty_down(window);
			}
			else {
				move_empty_up(window);
			}
		}
	}
	else {
		if (cp_empty_pos.i == cp_req_cube_pos.i) {
			if (cp_empty_pos.i == b_side_size - 1) {
				move_empty_up(window);
			}
			else {
				move_empty_down(window);
			}
		}
		else {
			move_empty_left(window);
		}
	}

	if (cp_req_cube_pos.j == cp_home_pos.j) {
		b_moving_method = MOVING_METHOD::NONE;
	}
}
void Bot::move_req_right_to_pos(sf::RenderWindow& window) {
	
	if (cp_empty_pos.j > cp_req_cube_pos.j) {
		if (cp_empty_pos.i == cp_req_cube_pos.i) {
			move_empty_left(window);
		}
		else {
			if (cp_empty_pos.i < cp_req_cube_pos.i) {
				move_empty_down(window);
			}
			else { 
				move_empty_up(window);
			}
		}
	}
	else {
		if (cp_empty_pos.i == cp_req_cube_pos.i) {
			if (cp_empty_pos.i == b_side_size - 1) {
				move_empty_up(window);
			}
			else {
				move_empty_down(window);
			}
		}
		else {
			move_empty_right(window);
		}
	}

	if (cp_req_cube_pos.j == cp_home_pos.j) {
		b_moving_method = MOVING_METHOD::NONE;
	}

}
void Bot::move_req_up_to_pos(sf::RenderWindow& window) {

	if (cp_empty_pos.i < cp_req_cube_pos.i) {
		if (cp_empty_pos.j == cp_req_cube_pos.j) {
			move_empty_down(window);
		}
		else {
			if (cp_empty_pos.j > cp_req_cube_pos.j) {
				move_empty_left(window);
			}
			else {
				move_empty_right(window);
			}
		}
	}
	else {
		if (cp_empty_pos.j == cp_req_cube_pos.j) {
			if (cp_empty_pos.j == b_side_size - 1) {
				move_empty_left(window);
			}
			else {
				move_empty_right(window);
			}
		}
		else {
			move_empty_up(window);
		}
	}

	if (cp_req_cube_pos.i == cp_home_pos.i) {
		b_moving_method = MOVING_METHOD::NONE;
	}
}
void Bot::assemble_row_exept_last_cube(sf::RenderWindow& window) {

	if (cp_req_cube_pos == cp_home_pos) {
		b_stage = STAGE::NONE;
		return;
	}

	if (cp_empty_pos.i <= cp_home_pos.i) {
		if (cp_empty_pos.j < cp_req_cube_pos.j) {
			b_moving_method = MOVING_METHOD::REQ_LEFT_TO_POS;
			move_req_left_to_pos(window);
		}
		else {
			move_empty_down(window);
		}
		return;
	}
	else {
		if (cp_req_cube_pos.j < cp_home_pos.j) {
			b_moving_method = MOVING_METHOD::REQ_RIGHT_TO_POS;
			move_req_right_to_pos(window);

		}
		else if (cp_req_cube_pos.j > cp_home_pos.j) {
			if (cp_req_cube_pos.i == cp_home_pos.i && (cp_req_cube_pos.j - cp_empty_pos.j) > 1) {
				move_empty_right(window);
			} 
			else {
				b_moving_method = MOVING_METHOD::REQ_LEFT_TO_POS;
				move_req_left_to_pos(window);
			}
		}
		else {
			if (cp_empty_pos.j < cp_req_cube_pos.j && cp_req_cube_pos.i - cp_home_pos.i < 2) {
				b_moving_method = MOVING_METHOD::EMPTY_UNDER_REQ;
				placed_empty_under_req(window);
			}
			else {
				b_moving_method = MOVING_METHOD::REQ_UP_TO_POS;
				move_req_up_to_pos(window);
			}
		}
		return;
	}
}
void Bot::placed_empty_under_req(sf::RenderWindow& window) {
	
	if (cp_empty_pos.i <= cp_req_cube_pos.i) {
		move_empty_down(window);
	}
	else {
		move_empty_right(window);
	}

	if (cp_empty_pos.j == cp_req_cube_pos.j) {
		b_moving_method = MOVING_METHOD::NONE;
	}
}
void Bot::place_empty_on_right_of_req(sf::RenderWindow& window) {
	if (cp_empty_pos.j > cp_home_pos.j - 1) {
		move_empty_left(window);
	}
	else if (cp_empty_pos.j < cp_home_pos.j - 1) {
		move_empty_right(window);
	}
	else if (cp_empty_pos.i > cp_home_pos.i) {
		move_empty_up(window);
	}
	else if (cp_empty_pos.i < cp_home_pos.i) {
		move_empty_down(window);
	}

	if (cp_empty_pos.i == cp_home_pos.i && cp_empty_pos.j == cp_home_pos.j - 1) {
		b_moving_method = MOVING_METHOD::NONE;
	}
}

void Bot::placed_last_cube(sf::RenderWindow& window) {
	bool go_to_next_step = false;


	switch (b_av.stage_step) {
	case 1:
		if (cp_req_cube_pos.i > cp_home_pos.i) {
			b_moving_method = MOVING_METHOD::REQ_UP_TO_POS;
			move_req_up_to_pos(window);
			break;
		}
		++b_av.stage_step;
	case 2:
		if (cp_req_cube_pos.j < cp_home_pos.j) {
			b_moving_method = MOVING_METHOD::REQ_RIGHT_TO_POS;
			move_req_right_to_pos(window);
			break;
		}
		++b_av.stage_step;
	case 3:
		if (cp_empty_pos.i != cp_req_cube_pos.i || cp_empty_pos.j != cp_req_cube_pos.j - 1) {
			b_moving_method = MOVING_METHOD::EMPTY_ON_RIGHT_FROM_REQ;
			place_empty_on_right_of_req(window);
		break;
		}
		++b_av.stage_step;
	case 4:
		b_moving_method = MOVING_METHOD::MIDDLE_CIRCLE;
		b_av.method_step_num = 1;
		b_av.clockwise = true;
		b_av.start_pos_at_the_top = false;
		middle_circle_movement(window);
		++b_av.stage_step;
		break;
	case 5:
		b_moving_method = MOVING_METHOD::LITTLE_CIRCLE;
		b_av.little_circle_quadrant = 2;
		b_av.method_step_num = 1;
		b_av.clockwise = false;
		little_circle_movement(window);
		++b_av.stage_step;
		break;
	default:
		break;
	}
	if (b_av.stage_step > 5) {
		cp_home_pos = cp_temp_pos;
		b_stage = STAGE::NONE;
	}
}

void Bot::middle_circle_movement(sf::RenderWindow& window) {
		switch (b_av.method_step_num)
		{
		case 1:
			b_av.start_pos_at_the_top ?
				(b_av.clockwise ? move_empty_right(window) : move_empty_left(window)) :
				(b_av.clockwise ? move_empty_left(window) : move_empty_right(window));
			break;
		case 2:
			b_av.start_pos_at_the_top ? move_empty_down(window) : move_empty_up(window);
			break;
		case 3:
		case 4:
			b_av.start_pos_at_the_top ?
				(b_av.clockwise ? move_empty_left(window) : move_empty_right(window)) :
				(b_av.clockwise ? move_empty_right(window) : move_empty_left(window));
			;
			break;
		case 5:
			b_av.start_pos_at_the_top ? move_empty_up(window) : move_empty_down(window);
			break;
		case 6:
			b_av.start_pos_at_the_top ?
			(b_av.clockwise ? move_empty_right(window) : move_empty_left(window)) :
			(b_av.clockwise ? move_empty_left(window) : move_empty_right(window));
			break;
		default:
			break;
		}

	if (b_av.method_step_num == 6)
	{
		b_av.method_step_num = 0;
		b_moving_method = MOVING_METHOD::NONE;
	}
	else {
		++b_av.method_step_num;
	}
}

void Bot::little_circle_movement(sf::RenderWindow& window) {

	if (b_av.clockwise) {
		b_av.little_circle_quadrant == 1 ? b_av.little_circle_quadrant = 4 : --b_av.little_circle_quadrant;
	}

	switch (b_av.little_circle_quadrant)
	{
	case 1:
		move_empty_right(window);
		break;
	case 2:
		move_empty_up(window);
		break;
	case 3:
		move_empty_left(window);
		break;
	case 4:
		move_empty_down(window);
		break;
	default:
		break;
	}

	++b_av.method_step_num;
	if (b_av.method_step_num > 4) {
		b_moving_method = MOVING_METHOD::NONE;
		b_av.method_step_num = 0;
	}
	else {
		if (!b_av.clockwise) {
			b_av.little_circle_quadrant == 4 ? b_av.little_circle_quadrant = 1 : ++b_av.little_circle_quadrant;
		}
	}
}

//Поиск кубика и возврат в caller при нахождени, без каких-либо проверок (в отличии от search_cubes_and_pos). !Функция оптимизирована только под последние две строки!
void Bot::search_certain_cube_and_pos() {

	cp_home_pos.i = (b_req_cube_value - 1) / b_side_size;
	cp_home_pos.j = b_req_cube_value - (cp_home_pos.i * b_side_size) - 1;

	for (int i = b_side_size - 2; i < b_side_size; i++) {
		for (int j = 0; j < b_side_size; j++) {
			if (b_board(i,j) == b_req_cube_value) {
				cp_req_cube_pos = { i,j };
				return;
			}
		}
	}
}

void Bot::assemble_last_two_rows(sf::RenderWindow& window) {

}



bool Bot::move_cube_to_given_pos(sf::RenderWindow& window, Cube_position& given_pos) {
	cp_home_pos = given_pos;
	if (cp_req_cube_pos.i > cp_home_pos.i) {
		move_req_up_to_pos(window);
		return false;
	}
	else if (cp_req_cube_pos.j > cp_home_pos.j) {
		move_req_left_to_pos(window);
		return false;
	}
	else if (cp_req_cube_pos.j < cp_home_pos.j) {
		move_req_right_to_pos(window);
		return false;
	}
	return true;
}
