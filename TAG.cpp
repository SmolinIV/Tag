//См. примечания и замечания в конце

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Constants.h"


void init_text(sf::Text& text, float xpos, float ypos, sf::String str, int size_font = 60,
	sf::Color menu_text_color = sf::Color::White, int bord = 0, sf::Color border_color = sf::Color::Black);

int main()
{
	// Задание размеров окна
	float w_width = 1440,//sf::VideoMode::getDesktopMode().width/2,
		w_height = 800;	 //sf::VideoMode::getDesktopMode().height/2;

	sf::RenderWindow window(sf::VideoMode(w_width, w_height), L"Пятнашки", sf::Style::Default);

	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	//window.setMouseCursorVisible(false);

	//Задний фон игры
	sf::RectangleShape backgrond(sf::Vector2f(w_width, w_height));
	sf::Texture texture_background;
	if (!texture_background.loadFromFile("png/bg1.png")) { return 1; }
	backgrond.setTexture(&texture_background);

	//Шрифт для названия игры
	sf::Font font;
	if (!font.loadFromFile("font/acsiomasupershockc.otf")) { return 2; }
	sf::Text title;
	title.setFont(font);
	init_text(title, (w_width / 2), (w_height / 7), L"Пятнашки", 80, sf::Color(APRICOT), 3);

	Board player_board;
	bool is_moving = false;

	while (window.isOpen())
	{

		// Обработка действий
		sf::Event event;	
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.key.code == sf::Mouse::Left && !is_moving) {
					sf::Vector2i click_pos = sf::Mouse::getPosition(window);
					is_moving = player_board.find_click_location(window, click_pos);
				}
				break;
			default:
				break;
			}
		}
		do {
			window.clear();
			window.draw(backgrond);
			window.draw(title);
			player_board.draw(window, window.getSize().x / 2, window.getSize().y / 2, is_moving);	//is_moving обнуляется внутри функции
			window.display();
		} while (is_moving);
	}

	return 0;
}



void init_text(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font,
	sf::Color menu_text_color, int bord, sf::Color border_color)
{
	mtext.setCharacterSize(size_font);
	mtext.setString(str);
	mtext.setFillColor(menu_text_color);
	mtext.setOutlineThickness(bord);
	mtext.setOutlineColor(border_color);
	mtext.setPosition(xpos - (mtext.getGlobalBounds().width / 2.0f), ypos - mtext.getGlobalBounds().height / 2.0f);
}


//Примечания: конструкторы копирования не используются, т.к. достаточно тех, что будут созданы по умолчанию, т.к. нет динамического выделения памяти
// 
// Замечания.
// 1. Не забудь преобразовать проверку подгрузки картинок
// 
// 
// 
// 
// 
//