#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constants.h"
#include "Board.h"

class Bot {
private:
	Board b_board;			// Игровое поле компьютера
	float b_res_ftime;		// Затраченное на сборку время
	int b_side_size;		// Размер поля компьютера
	int b_game_moves;		// Количество ходов компьютера
	bool b_assemble_done;	// Флаг успешного завершения сборки
	bool b_is_moving;		// Флаг наличия движения кубика на поле
	bool b_cube_placed;		// Флаг подтверждения установки кубика на своё место
	int b_req_cube_value;	// Цифровое значение кубика, к которому применяются алгоритмы для его постановки на место (далее - искомый кубик)

	struct AUXILIARY_VARIABLES {		// Вспомогательные переменные
		int method_step_num;			// Шаг метода
		int stage_step;					// Шаг этапа
		int little_circle_quadrant;		// Номер квадратна, который будет описывать "пустая клетка"
		bool clockwise;					// Флаг движения по часовой стрелки для " пустой клетки"
		bool start_pos_at_the_top;		// Флаг нахождения "пустой клетки" в верхней точке описыввемого круга
	} b_av;

	enum class STAGE {		// Этапы сборки
		ASSEMBLE_ROW,		// Сборка линии без последнего кубика
		PLACED_LAST_CUBE,	// Постановка на место последнего в ряду кубика
		ASSEMBLE_LAST_ROWS,	// Сборка последних двух рядов игрового поля
		NONE
	} b_stage;

	enum class MOVING_METHOD {		// Конкретные алгоритмы для сборки
		EMPTY_UNDER_REQ,			// Установка пустой клетки под искомый кубик
		EMPTY_ON_RIGHT_FROM_REQ,	// Установка пустой клетки справа от искомого кубика
		REQ_UP_TO_POS,				// Перемещение искомого кубика вверх до нужной линии
		REQ_LEFT_TO_POS,			// Перемещение кубика влево до нужного столбца
		REQ_RIGHT_TO_POS,			// Перемещение кубика вправо до нужного столбца
		LITTLE_CIRCLE,				// Движение пустой клетки по "малому" кругу (по одному шагу в каждую сторону)
		MIDDLE_CIRCLE,				// Движение пустой клетки по "среднему" кругу ( 1 клетка по высоте, 3 по длине)
		NONE
	} b_moving_method;


	struct Cube_position { // Структоура хранения позиции кубиков с перегруженными операторами сравнения
		int i, j; 

		bool operator==(Cube_position& other) {
			return (this->i == other.i && this->j == other.j);
		}

		bool operator!=(Cube_position& other) {
			return !(this->i == other.i && this->j == other.j);
		}

	} cp_empty_pos, cp_req_cube_pos, cp_home_pos, cp_temp_pos;		// Соответсвенно пустая клетка, искомый кубик, место искомого кубика (дом), временная позиция

	// Поиск кубика и его дома (функция заканичвает работу, только если находит кубик, не стоящий на месте)
	void search_cubes_and_pos();
	// Поиск заданного внутри класса кубика и его дома
	void search_certain_cube_and_pos();

	//Движения пустой клетки в разные стороны
	void move_empty_up(sf::RenderWindow& window);
	void move_empty_down(sf::RenderWindow& window);
	void move_empty_left(sf::RenderWindow& window);
	void move_empty_right(sf::RenderWindow& window);

	// Движение пустой клетки под искмый кубик
	void placed_empty_under_req(sf::RenderWindow& window);
	// Движение пустой клетки в позицию справа от искомого
	void place_empty_on_right_of_req(sf::RenderWindow& window);

	// Перемещение искомого кубика к дому
	void move_req_up_to_pos(sf::RenderWindow& window);
	void move_req_left_to_pos(sf::RenderWindow& window);
	void move_req_right_to_pos(sf::RenderWindow& window);

	//Сборка линии без последнего кубика
	void assemble_row_exept_last_cube(sf::RenderWindow& window);
	// Постановка последнего кубика на своё место
	void placed_last_cube(sf::RenderWindow& window);
	//Сборка последних двух рядов
	void assemble_last_two_rows(sf::RenderWindow& window);
	//Движение по среднему кругу (например, "влево, вверх, три раза вправо, вниз, влево). Движение может быть по часовой и против, вверх и вниз
	void middle_circle_movement(sf::RenderWindow& window);
	// Движение по "малому" кругу в заданном квадранте. Пустая клетка как-бы стоит в системе координат в точке 0,0. В зависимости от заданного направления двигается по одному из квадрантов
	void little_circle_movement(sf::RenderWindow& window);

	// Перемещение искомого кубика в заданную позицию
	bool move_cube_to_given_pos(sf::RenderWindow& window, Cube_position& given_pos);
	

public:
	Bot();
	Bot(Board& board);

	// Получить время выполнения в секундах
	float get_ftime_res() const { return b_res_ftime; }
	// Получить количество затраченных компьютером ходов
	int get_moves() { return b_game_moves; }
	// Записать время выполнения в секундах 
	void set_ftime_res(float res) { b_res_ftime = res; };
	// Итератор ходов компьютера
	void new_move() { ++b_game_moves; }
	// Получить доступ к доске компьютера
	Board& board() { return b_board; }
	// Получить состояние флага собранного игрового поля
	bool assemble_done() { return b_assemble_done; }
	// Получить значение флага наличия перемещений кубиков по игровому полю
	bool* get_permit_to_move() { return &b_is_moving; }
	// Сформировать нааальные данные о состоянии доски (где пустая клетка, кубик с цифрой 1 и его дом)
	void set_start_pos();
	// Сбросить результаты игры компьютера
	void reset_results();
	// Запусть алгоритм сборки головоломки
	void assemble_board(sf::RenderWindow& window);

};
