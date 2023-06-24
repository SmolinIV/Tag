#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Game_time {
private:
	float gt_ftime;					// Количество прошедшего времени в секундах
	int gt_iseconds;				// Количество секунд в формате ЧЧ:ММ:СС
	int gt_iminutes;				// Количество минут в формате ЧЧ:ММ:СС
	int gt_ihours;					// Количество часов в формате ЧЧ:ММ:СС
	sf::Clock gt_clock;				// объект класса время
	sf::Text gt_time_in_text;		// Текстурное представление времени
	sf::Font gt_font;				// Шрифт текстуры времени
	std::string gt_strtime;			// Время в строковом формате (ЧЧ:ММ:СС)

	sf::Vector2f gt_position{0, 0};	// Положение текстуры времени

public:
	Game_time(std::error_code& syst_error);
	// Обнулить таймер
	void restart() { gt_clock.restart(); }
	// Получить последнее зафиксированное количество прошедших секунд
	float get_ftime() { return gt_ftime; };
	// Получить строковое представление времени в формате ЧЧ:ММ:СС
	std::string& const get_strtime() { return gt_strtime; }
	// Задание всех параметров текстуры текста
	void init_text();
	// Зафиксировать текущее время (общее и для формата ЧЧ:ММ:СС)
	void init_time();
	// Задать позицию времени на экране
	void set_position(float xpos, float ypos);
	//Отрисовать время
	void draw(sf::RenderWindow& window);
};

