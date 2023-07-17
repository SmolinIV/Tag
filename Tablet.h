#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Tablet {

private:
	sf::RectangleShape t_tablet;				// Объект таблички
	sf::Vector2f t_tablet_size = { 400,70 };	// Размеры таблички
	sf::Vector2f t_tablet_pos;					// Положение таблички на экране
	sf::Texture t_tablet_texture;				// Текстура таблички
	sf::Text t_tablet_text;						// Текстурный текст таблички
	sf::Font t_font;							// Шрифт текста

	int t_text_size = 40;
public:
	Tablet(sf::String text, sf::Vector2f pos);

	//Получить текущее положение таблички
	sf::Vector2f get_pos() { return t_tablet_pos; }
	// Получить размеры таблички
	sf::Vector2f get_size() { return t_tablet_size; }
	//Отрисовать табличку
	void draw(sf::RenderWindow& window);
	// Сменить цвет текста таблички
	void change_color(sf::Color color);

};
