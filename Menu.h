#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Constants.h"
#include "Tablet.h"

class Menu {
private:

	std::vector<Tablet*> m_menu;
	sf::Vector2f m_pos;

	float m_step = 80;
	int selected_point = 0;

public:

	Menu(sf::RenderWindow& window, std::initializer_list <sf::String> points);
	
	void draw(sf::RenderWindow& window);

	void menu_navigating();

	void moving_up();


	~Menu() {
		for (int i = 0; i < m_menu.size(); i++)
		{
			delete m_menu[i];
			m_menu[i] = nullptr;
		}
	}
};

Menu::Menu(sf::RenderWindow& window, std::initializer_list <sf::String> list) {
	m_pos.x = window.getSize().x / 2;
	m_pos.y = window.getSize().y / 2.5;
	m_menu.reserve(list.size());
	for (int i = 0; i < list.size(); i++) {
		Tablet* t = new Tablet{ *(list.begin() + i), {m_pos.x, m_pos.y + m_step * i} };
		m_menu.push_back(t);
		if (i == selected_point) {
			m_menu[i]->selected(sf::Color(ORANGE_RED));
		}
		t = nullptr;
	}

}

void Menu::draw(sf::RenderWindow& window) {
	for (auto item : m_menu) {
		(*item).draw(window);
	}
}

void Menu::moving_up() {
	
}