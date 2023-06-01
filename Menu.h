#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <array>
class Menu
{
private:
	sf::RectangleShape m_tablet;
	sf::Texture m_tablet_texture;
	sf::Vector2f m_first_tablet_pos;
	sf::Text m_item_text;
	sf::Font m_font;
	static const int m_items_number = 3;
	int m_current_item;
	std::array<std::string, m_items_number> m_menu_items {"Начать игру", "Настройки", "Выход"};

public:
	Menu();

	void draw(sf::RenderWindow& window);

	void menu_navigating();


	~Menu() {}
};

Menu::Menu() {

}

