//См. примечания и замечания в конце
#include <iostream>
#include <SFML/Graphics.hpp>
#include <system_error>
#include "Board.h"
#include "Constants.h"
#include "Menu.h"
#include <SFML/Window.hpp>
#pragma warning(disable : 4996)


void init_text(sf::Text& text, float xpos, float ypos, sf::String str, int size_font = 60,
	sf::Color menu_text_color = sf::Color(RGB_APRICOT), int bord = 3, sf::Color border_color = sf::Color(RGB_DARK_BROWN));

MENU_POINTS work_with_main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& ERROR);
void work_with_postgame_menu(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error, GAMING_RESULT res);
void shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error);
GAMING_RESULT player_gaming(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error);

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
		if (!texture_background.loadFromFile("png/bg2.png")) { syst_error = std::make_error_code(std::errc::no_such_file_or_directory); break; }

		Board player_board(syst_error);

		do {
			background.setTexture(&texture_background);

			if (work_with_main_menu(window, background, syst_error) == MENU_POINTS::EXIT || syst_error) { break; }

			shuffle_board(window, background, player_board, syst_error);
			if (syst_error) { break; }

			player_gaming(window, background, player_board, syst_error);
			if (syst_error) { break; }
			sf::Image I_pg;
			sf::Texture postgame_screen;
			window.capture();
			postgame_screen.create(w_width, w_height);
			postgame_screen.update(window);
			I_pg = postgame_screen.copyToImage();
			postgame_screen.loadFromImage(I_pg);
			background.setTexture(&postgame_screen);
			work_with_main_menu(window, background, syst_error);
		} while (true);
		if (syst_error) { break; }
	} while (false);


	if (!syst_error) {
		std::cout << syst_error.message() << std::endl;
		return syst_error.value();
	}

	return 0;
}



MENU_POINTS work_with_main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error) {
	do {
		//Шрифт для названия игры
		sf::Text title;
		sf::Font font;
		if (!font.loadFromFile("font/acsiomasupershockc.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory); 
			return MENU_POINTS::EXIT;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y / 6, L"Пятнашки", 100, sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));

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
				if (choice == MENU_POINTS::START) { return choice; }
				//else if ((choice == MENU_POINTS::OPTINS);
				else { return choice; }
			}
		}
	} while (false);
}

void shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error) {
	do {
		sf::Text title;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			break;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y / 8, L"Приготовьтесь!", 80);
		bool is_moving = false;
		int shuffle_steps = 0;
		int number_of_swap = 10;
		while (window.isOpen() && shuffle_steps < number_of_swap)
		{
			if (shuffle_steps >= number_of_swap / 2 && (number_of_swap - shuffle_steps) % 10 == 0) {
				init_text(title, window.getSize().x / 2, window.getSize().y /8, std::to_string((number_of_swap - shuffle_steps) / 10) + "...", 80);
			}
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
				window.draw(title);
				player_board.draw(window, window.getSize().x / 2, window.getSize().y / 2, is_moving);	//is_moving обнуляется внутри функции
				window.display();
			} while (is_moving);
			is_moving = true;
			++shuffle_steps;
		}
	} while (false);
}

GAMING_RESULT player_gaming(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error) {
	do {
		sf::Text title;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			break;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y / 8, L"Начали!", 80);
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
				window.draw(title);
				player_board.draw(window, window.getSize().x / 2, window.getSize().y / 2, is_moving);	//is_moving обнуляется внутри функции
				window.display();
			} while (is_moving);
			if (player_board.sequence_restored()) {
				return GAMING_RESULT::PLAYER_WIN;
			}
		}
	} while (false);
	return GAMING_RESULT::PLAYER_LOSE;
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
// 2. Тесты.
// 3. Возможно стоит выделить прорисовку в отдельную функцию.
// 4. Убрать std::array из board, всё равное возможности не используются, только загружает код.
// 5. Перегрузить оператор сравнения для Cube.
// 6. Привязать размер кубиков к доске (по аналогии текска в кубиках/табличках), а так же размеры текстов
// 
// 
//