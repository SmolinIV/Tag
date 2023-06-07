//См. примечания и замечания в конце
#include <iostream>
#include <SFML/Graphics.hpp>
#include <system_error>
#include "Board.h"
#include "Constants.h"
#include "Menu.h"


/*void init_text(sf::Text& text, float xpos, float ypos, sf::String str, int size_font = 60,
	sf::Color menu_text_color = sf::Color::White, int bord = 0, sf::Color border_color = sf::Color::Black);*/

MENU_POINTS first_screen(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& ERROR);
void shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error);
void player_gaming(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error);

int main()
{
	std::error_code syst_error;
	do {
		srand(time(NULL));
		// Задание размеров окна
		float w_width = 1440,//sf::VideoMode::getDesktopMode().width/2,
			w_height = 800;	 //sf::VideoMode::getDesktopMode().height/2;

		sf::RenderWindow window(sf::VideoMode(w_width, w_height), L"Пятнашки", sf::Style::Default);

		window.setFramerateLimit(60);
		window.setVerticalSyncEnabled(true);
		//window.setMouseCursorVisible(false);

		//Задний фон игры
		sf::RectangleShape background(sf::Vector2f(w_width, w_height));
		sf::Texture texture_background;
		if (!texture_background.loadFromFile("png/bg2.png")) { syst_error = std::make_error_code(std::errc::no_such_file_or_directory);		break; }
		background.setTexture(&texture_background);

		Board player_board(syst_error);

		if (first_screen(window, background, syst_error) == MENU_POINTS::EXIT || syst_error)
		{
			break;
		}

		shuffle_board(window, background, player_board, syst_error);
		if (syst_error) { break; }

		player_gaming(window, background, player_board, syst_error);
		if (syst_error) { break; }
	} while (false);

	if (!syst_error) {
		std::cout << syst_error.message() << std::endl;
		return syst_error.value();
	}

	return 0;
}

/*void init_text(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font,
	sf::Color menu_text_color, int bord, sf::Color border_color)
{
	mtext.setCharacterSize(size_font);
	mtext.setString(str);
	mtext.setFillColor(menu_text_color);
	mtext.setOutlineThickness(bord);
	mtext.setOutlineColor(border_color);
	mtext.setPosition(xpos - (mtext.getGlobalBounds().width / 2.0f), ypos - mtext.getGlobalBounds().height / 2.0f);
}*/

MENU_POINTS first_screen(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error) {

	//Шрифт для названия игры
	sf::Text title;
	sf::Font font;
	if (!font.loadFromFile("font/acsiomasupershockc.otf")) { exit(1); }
	title.setFont(font);
	title.setCharacterSize(80);
	title.setString(L"Пятнашки");
	title.setFillColor(sf::Color(APRICOT));
	title.setOutlineThickness(3);
	title.setOutlineColor(sf::Color(DARK_BROWN));
	title.setPosition(window.getSize().x / 2- (title.getGlobalBounds().width / 2.0f), window.getSize().y / 7 - title.getGlobalBounds().height / 2.0f);

	Menu main_menu{ window,{L"Начать игру", L"Настройки", L"Выход"} };

	MENU_POINTS choice = MENU_POINTS::NONE;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Up:
					main_menu.move_up();
					break;
				case sf::Keyboard::Down:
					main_menu.move_down();
					break;
				case sf::Keyboard::Enter:
					choice = main_menu.selected();
				}

			default:
				break;
			}
		}
		window.clear();
		window.draw(background);
		window.draw(title);
		main_menu.draw(window);
		window.display();
		if (choice != MENU_POINTS::NONE) {
			if (choice == MENU_POINTS::START) {
				sf::Clock clock;
				float time = 0;
				clock.restart();
				while (time < 250) {
					time = clock.getElapsedTime().asMilliseconds();
				}
				return choice;
			}
			return choice;
		}
	}
}

void shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error) {
	sf::Clock clock;
	float time = 0;
	clock.restart();
	while (time < 550) {
		time = clock.getElapsedTime().asMilliseconds();
	}

	bool is_moving = false;
	int shuffle_steps = 0;
	while (window.isOpen() && shuffle_steps < 50)
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
			default:
				break;
			}
		}
		player_board.shaffle_board(window);
		do {
			window.clear();
			window.draw(background);
			player_board.draw(window, window.getSize().x / 2, window.getSize().y / 2, is_moving);	//is_moving обнуляется внутри функции
			window.display();
		} while (is_moving);
		is_moving = true;
		++shuffle_steps;
	}
}

void player_gaming(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error) {
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
					is_moving = player_board.ask_for_moving(window, click_pos, true);
				}
				break;
			default:
				break;
			}
		}
		do {
			window.clear();
			window.draw(background);
			player_board.draw(window, window.getSize().x / 2, window.getSize().y / 2, is_moving);	//is_moving обнуляется внутри функции
			window.display();
		} while (is_moving);
		if (player_board.sequence_restored()) {
			break;
		}
	}
}


//Примечания: конструкторы копирования не используются, т.к. достаточно тех, что будут созданы по умолчанию, т.к. нет динамического выделения памяти
// 
// Замечания.
// 1. Не забудь преобразовать проверку подгрузки картинок
// 2. Тесты.
// 3. Возможно стоит выделить прорисовку в отдельную функцию.
// 4. Убрать std::array из board, всё равное возможности не используются, только загружает код.
// 5. Перегрузить оператор сравнения для Cube.
// 6. Привязать размер кубиков к доске (по аналогии текска в кубиках/табличках), а так же размеры текстов
// 
// 
//