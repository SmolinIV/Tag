#pragma once
#include <SFML/Graphics.hpp>
#include <system_error>
#include <array>
#include "Cube.h"
#include "Constants.h"


class Board
{
private:
	float b_vert_bord;			// Размер вертикального борта 
	float b_horiz_bord;			// Размер горизонтального борта
	float b_width;				// Ширина игорового поля/доски (включая борта)
	float b_height;				// Высота игорового поля/доски (включая борта)
	sf::RectangleShape b_board;	// Сам объект игрового поля
	sf::Texture b_texture;		// Текстура поля
	sf::Vector2f b_board_pos;	// Позиция поля
	Cube* b_moving_cube;		// Указатель на кубик, который передвигается в момент обращения к доске.
	static const int b_side_size = 4;		// Размер стороны поля в кубиках (определяет размерность рятнашек)
	std::array<std::array<Cube, b_side_size>, b_side_size> b_cubes;		// Двумерный массив из кубиков, образующий матрицу 4х4
	struct cube_pos { int i, j; } b_empty_cube_pos,						// Положение пустого пространства
								  b_last_swap_cube;						// Положение последнего передвинутого кубика
	
	// Задать положение всех кубиков на поле
	void set_cubes_position();
public:
	Board();
	Board(std::error_code& ec);
	// Получить ширину поля
	float get_board_width() const { return b_width; }
	// Получить высоту поля
	float get_board_height() const { return b_height; }
	// Получить размерность поля (количество кубиков в ряду == столбце)
	int get_board_side_size() { return b_side_size; }
	// Задать позицию поля
	void setPosition(float xpos, float ypos);
	// Задать скорость перемещения кубиков внутри поля
	void set_smoothness(float koef);
	// Отрисовать поле
	void draw(sf::RenderWindow& window);
	// Произвести перемещение кубика перед отрисовкой всего поля
	void draw(sf::RenderWindow& window, bool &need_move);

	// Перемешать кубики на поле (перемешивание пошаговое, чтобы не нарушить последовательность)
	void shaffle_board(sf::RenderWindow& window);
	// Сменить значения позиций кубика и, соответственно, пустого пространства с предварительной проверкой на возможность этого перемешения
	bool ask_for_moving(sf::RenderWindow& window, sf::Vector2i click_pos, bool mouse_control = false);
	// Свап положений двух кубиков
	void swap_cubes(int i1, int j1, int i2, int j2 );
	// Проверить, восстановлена ли последовательность 
	bool sequence_restored();
	// Перегрузка оператора скобок для обращения к конкретному кубику (Проще в реализации и эффективнее с точ.зр. производителдьности, чем [])
	int operator()(unsigned int i, unsigned int j) { return b_cubes[i][j].get_cube_value(); }
	// Расположить кубики в начальное (т.е. собранное) положение
	void reset();

	~Board() {}

};