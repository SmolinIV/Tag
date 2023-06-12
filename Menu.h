#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Constants.h"
#include "Tablet.h"


class Menu {
private:

	std::vector<std::shared_ptr<Tablet>> m_menu;
	sf::Vector2f m_pos;

	float m_step = 100;
	int selected_point = 0;


public:

	Menu(sf::RenderWindow& window, std::initializer_list <sf::String> points);
	
	void draw(sf::RenderWindow& window);

	void menu_navigating();

	void move_up();

	void move_down();

	int selected();


	~Menu() {}
};

Menu::Menu(sf::RenderWindow& window, std::initializer_list <sf::String> list) {
	m_pos.x = window.getSize().x / 2;
	m_pos.y = window.getSize().y / 2.5;
	m_menu.reserve(list.size());
	for (int i = 0; i < list.size(); i++) {
		std::shared_ptr<Tablet> t{std::make_shared<Tablet>( *(list.begin() + i), sf::Vector2f{m_pos.x, m_pos.y + m_step * i} ) };
		m_menu.push_back(t);
		if (i == selected_point) {
			m_menu[i]->change_color(sf::Color(RGB_LIGHT_BROWN));
		}
		t = nullptr;
	}

}

void Menu::draw(sf::RenderWindow& window) {
	for (auto &item : m_menu) {
		(*item).draw(window);
	}
}

void Menu::move_up() {
	m_menu[selected_point]->change_color(sf::Color(RGB_BEIGE));

	if (selected_point == 0) {
		selected_point = m_menu.size() - 1;
	}
	else
	{
		--selected_point;
	}
	
	m_menu[selected_point]->change_color(sf::Color(RGB_LIGHT_BROWN));
	
}

void Menu::move_down() {
	m_menu[selected_point]->change_color(sf::Color(RGB_BEIGE));

	if (selected_point == m_menu.size() - 1) {
		selected_point = 0;
	}
	else
	{
		++selected_point;
	}

	m_menu[selected_point]->change_color(sf::Color(RGB_LIGHT_BROWN));

}

int Menu::selected() {
	return selected_point;
}