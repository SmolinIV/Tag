#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Constants.h"
#include "Tablet.h"


class Menu {
private:

	std::vector<std::shared_ptr<Tablet>> m_menu;		// Массив пунктов меню
	sf::Vector2f m_pos;			// Позиция меню
	float m_step = 100;			// Шаг между табличками
	int selected_point = 0;		// Выбранный пункт меню

public:

	Menu(sf::RenderWindow& window, std::initializer_list <sf::String> points);
	
	void draw(sf::RenderWindow& window);
	// Движение по меню курсором
	void menu_navigating(sf::Vector2i click_pos);
	// Выбрать следующую табличку (при урпавлении стрелками)
	void move_up();
	// Выбрать предыдущую табличку (при управлении стрелками)
	void move_down();
	// Получить номер (индекс) выбранной ячейки
	int get_selected();
	// Вернуть номер ячейки, на которую кликнули
	int click_point(sf::Vector2i click_pos);

	~Menu() {}


};
