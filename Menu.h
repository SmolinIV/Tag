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


	void move_up();

	void move_down();

	int selected();

	void menu_navigating(sf::Vector2i click_pos);

	int click_point(sf::Vector2i click_pos);

	~Menu() {}


};
