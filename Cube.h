#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"


class Cube
{
private:
	float c_side = 100.0f;			// Размеры кубика
	sf::Vector2f c_current_pos;		// Текущее положение кубика
	sf::Vector2f c_new_pos;			// Целевое положение кубика
	int c_cube_value;				// Цифра кубика
	float c_koef_smoothness;		// Скорость перемещения кубика
	sf::RectangleShape c_cube;		// Сам кубик, как объект класса
	sf::Texture c_texture;			// Текстура кубика
	sf::Text c_number;				// Цифра (для отрисовки в кубике)
	sf::Font c_font;				// Шрифт цифры
	DIRECTION c_dir;				// Заданное направление перемещения кубика (вверх, вниз, ... )

public:

    Cube();
	Cube(int i);
	// Получить размер кубика
	float get_size() const { return c_side; }
	//Получить цифровое значение кубика
	int get_cube_value() const { return c_cube_value; }
	//Задать положение кубика (задаётся теущее положение (c_current_pos)
	void set_position(float x, float y) { c_current_pos = { x,y }; }
	//Отрисовать кубик
	void draw(sf::RenderWindow& window);
	//Задать новую позицию для кубика (это не перемещение текстуры!)
	void change_pos(DIRECTION dir);
	//Задать скорость перемещения кубика
	void set_smoothness(float koef) { c_koef_smoothness = koef; }
	// Переместить тектуру кубика в заданную позицию (если кубик на месте, флаг необходимости движения станет false, разрешая дальнейшие операции с доской/кубиками)
	void moving_cube(bool& need_move);
	// "Сбросить" заданное значение кубика на вновь заданное (используется для быстрого восстановления последвательности игровой доски)
	void reset(int i);

	~Cube() {}
};
