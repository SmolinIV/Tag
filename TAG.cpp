//См. примечания и замечания в конце
#include <iostream>
#include <SFML/Graphics.hpp>
#include <system_error>
#include "Board.h"
#include "Constants.h"
#include "Menu.h"
#include <SFML/Window.hpp>
#include "game_time.h"
#include "Player.h"
#include "bot.h"
#pragma warning(disable : 4996)


void init_text(sf::Text& text, float xpos, float ypos, sf::String str, int size_font = 60,
	sf::Color menu_text_color = sf::Color(RGB_APRICOT), int bord = 3, sf::Color border_color = sf::Color(RGB_DARK_BROWN));

PROCESS_STEPS work_with_main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error);
PROCESS_STEPS work_with_postgame_menu(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAMING_RESULT& res);
PROCESS_STEPS shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error);
PROCESS_STEPS player_gaming(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAMING_RESULT& res);
PROCESS_STEPS player_vs_bot(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, Bot& bot, std::error_code& syst_error, GAMING_RESULT& res);

void move_boards_apart(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, Bot& bot);

int main()
{
	setlocale(LC_ALL, "RUS");
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
		background.setTexture(&texture_background);

		Board player_board(syst_error);
		player_board.setPosition(window.getSize().x / 2, window.getSize().y / 2);

		Player player(player_board);
		Bot bot;

		PROCESS_STEPS next_step = PROCESS_STEPS::MAIN_MENU;
		GAMING_RESULT player_result = GAMING_RESULT::GAME_PAUSE;

		do {
			switch (next_step)
			{
			case PROCESS_STEPS::MAIN_MENU:
				next_step = work_with_main_menu(window, background, syst_error);
				break;

			case PROCESS_STEPS::START_WITH_SHAFFLING:
				next_step = shuffle_board(window, background, player.board(), syst_error);
				break;

			case PROCESS_STEPS::START_PLAYING:
				next_step = player_gaming(window, background, player, syst_error, player_result);
				break;

			case PROCESS_STEPS::GO_TO_POSTGAME_MENU:
				next_step = work_with_postgame_menu(window, background, player, syst_error, player_result);
				break;

			//case PROCESS_STEPS::GO_TO_OPTIONS:
			//	break;

			case PROCESS_STEPS::START_PLAING_WITH_BOT:
				next_step = player_vs_bot(window, background, player, bot, syst_error, player_result);
			case PROCESS_STEPS::EXIT:
				next_step = PROCESS_STEPS::EXIT;
				break;

			case PROCESS_STEPS::NONE:
			default:
				syst_error = std::make_error_code(std::errc::protocol_error);
				next_step = PROCESS_STEPS::EXIT;
				break;
			}
		} while (next_step != PROCESS_STEPS::EXIT );
	} while (false);


	if (!syst_error) {
		if (syst_error == std::errc::protocol_error) {
		std::cout << "Something went wrong..." << std::endl;
		}
		else {
			std::cout << syst_error.message() << std::endl;
			return syst_error.value();
		}
	}
}

PROCESS_STEPS work_with_main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error) {

	//Шрифт для названия игры
	sf::Text title;
	sf::Font font;
	if (!font.loadFromFile("font/acsiomasupershockc.otf")) {
		syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
		return PROCESS_STEPS::EXIT;
	}
	title.setFont(font);
	init_text(title, window.getSize().x / 2, window.getSize().y / 6, L"Пятнашки", 100, sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));

	Menu main_menu{ window,{L"Начать игру", L"Настройки", L"Выход"} };
	int menu_point_number = -1;
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
					menu_point_number = main_menu.selected();
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
		switch (menu_point_number) {
		case 0: return PROCESS_STEPS::START_WITH_SHAFFLING;
		case 1: return PROCESS_STEPS::GO_TO_OPTIONS; 
		case 2: return PROCESS_STEPS::EXIT;
		default:
			break;
		}
	}
}

PROCESS_STEPS shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error) {
	sf::Text title, go_to_menu;
	sf::Font font;
	if (!font.loadFromFile("font/troika.otf")) {
		syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
		return PROCESS_STEPS::EXIT;
	}
	title.setFont(font);
	init_text(title, window.getSize().x / 2, window.getSize().y / 8, L"Приготовьтесь!", 80);

	go_to_menu.setFont(font);
	init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"Для возврата в главное меню нажмите ESC", 20);
	bool is_moving = false;
	int shuffle_steps = 0;
	int number_of_swap = 60;
	while (window.isOpen() && shuffle_steps < number_of_swap) {
		if (shuffle_steps >= number_of_swap / 2 && (number_of_swap - shuffle_steps) % 10 == 0) {
			init_text(title, window.getSize().x / 2, window.getSize().y / 8, std::to_string((number_of_swap - shuffle_steps) / 10) + "...", 80);
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
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) {
					player_board.reset();
					return PROCESS_STEPS::MAIN_MENU; }
			default:
				break;
			}
		}
		do {
			window.clear();
			window.draw(background);
			window.draw(title);
			window.draw(go_to_menu);
			player_board.draw(window, is_moving);	//is_moving обнуляется внутри функции
			window.display();
		} while (is_moving);
		++shuffle_steps;
		if (shuffle_steps < number_of_swap) {
			player_board.shaffle_board(window);
		}
		is_moving = true;
	}

	return PROCESS_STEPS::START_PLAYING;
}

PROCESS_STEPS player_gaming(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAMING_RESULT& res) {
	player.reset_results();
	do {
		sf::Text title, go_to_menu;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y / 8, L"Начали!", 80);
		go_to_menu.setFont(font);
		init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"Для возврата в главное меню нажмите ESC", 20);

		Game_time running_time(syst_error);
		if (syst_error) { return PROCESS_STEPS::EXIT; }
		running_time.set_position(window.getSize().x - 200, 30);

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
						is_moving = player.board().ask_for_moving(window, click_pos, true);
						if (is_moving) { player.new_move(); }
					}
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape) {
						player.board().reset();
						res = GAMING_RESULT::PLAYER_LOSE;
						return PROCESS_STEPS::MAIN_MENU;
					}
				default:
					break;
				}
			}
			do {
				window.clear();
				window.draw(background);
				window.draw(title);
				running_time.draw(window);
				player.board().draw(window, is_moving);	//is_moving обнуляется внутри функции
				window.draw(go_to_menu);
				window.display();
			} while (is_moving);
			if (player.board().sequence_restored()) {
				res = GAMING_RESULT::PC_WIN;
				player.set_fresult(running_time.get_ftime());
				player.set_strresult(running_time.get_strtime());
				return PROCESS_STEPS::GO_TO_POSTGAME_MENU;
			}
		}
	} while (false);
	return PROCESS_STEPS::EXIT;
}

PROCESS_STEPS work_with_postgame_menu(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAMING_RESULT& res) {

	/*	sf::RectangleShape deck(sf::Vector2f(500, 500));
		deck.setPosition(window.getSize().x / 2 - deck.getGlobalBounds().width/2, window.getSize().y / 2 - deck.getGlobalBounds().height / 2);
		sf::Texture deck_texture;
		if (!deck_texture.loadFromFile("png/ended_deck1.png")) { 
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory); 
			return PROCESS_STEPS::EXIT;
		}
		deck.setTexture(&deck_texture);*/

		//Шрифт для результата игры
		sf::Text result, time_res;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) { 
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT; }

		result.setFont(font);
		time_res.setFont(font);
		
		init_text(result, window.getSize().x / 2, window.getSize().y / 7, L"Победа!", 100, sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));
		init_text(time_res, window.getSize().x / 2, window.getSize().y / 4, L"Время выполнения: " + (sf::String)player.get_strtime_res(), 50,
			      sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));

		Menu main_menu{ window,{L"Сыграть ещё раз", L"Настройки", L"Выход"} };

		int menu_point_number = -1;
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
						menu_point_number = main_menu.selected();
					}

				default:
					break;
				}
			}
			window.clear();
			window.draw(background);
			//window.draw(deck);
			window.draw(result);
			window.draw(time_res);
			main_menu.draw(window);
			window.display();
			switch (menu_point_number) {
			case 0: return PROCESS_STEPS::START_WITH_SHAFFLING;
			case 1: return PROCESS_STEPS::GO_TO_OPTIONS;
			case 2: return PROCESS_STEPS::EXIT;
			default:
				break;
			}
		}
}

PROCESS_STEPS player_vs_bot(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, Bot& bot, std::error_code& syst_error, GAMING_RESULT& res) {
	player.reset_results();
	bot.reset_results();

	bot.board() = player.board();
	move_boards_apart(window, background, player, bot);
	do {
		sf::Text title, go_to_menu;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y / 8, L"Начали!", 80);
		go_to_menu.setFont(font);
		init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"Для возврата в главное меню нажмите ESC", 20);

		Game_time running_time(syst_error);
		if (syst_error) { return PROCESS_STEPS::EXIT; }
		running_time.set_position(window.getSize().x - 200, 30);

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
						is_moving = player.board().ask_for_moving(window, click_pos, true);
						if (is_moving) { player.new_move(); }
					}
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape) {
						player.board().reset();
						res = GAMING_RESULT::PLAYER_LOSE;
						return PROCESS_STEPS::MAIN_MENU;
					}
				default:
					break;
				}
			}
			do {
				window.clear();
				window.draw(background);
				window.draw(title);
				running_time.draw(window);
				player.board().draw(window, is_moving);	//is_moving обнуляется внутри функции
				window.draw(go_to_menu);
				window.display();
			} while (is_moving);
			if (player.board().sequence_restored()) {
				res = GAMING_RESULT::PC_WIN;
				player.set_fresult(running_time.get_ftime());
				player.set_strresult(running_time.get_strtime());
				return PROCESS_STEPS::GO_TO_POSTGAME_MENU;
			}
		}
	} while (false);
	return PROCESS_STEPS::EXIT;
}

void move_boards_apart(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, Bot& bot) {

	float xpos_player = window.getSize().x / 2;
	float xpos_bot = xpos_player;
	float ypos_player = window.getSize().y / 2;
	float ypos_bot = ypos_player;
	//sf::Vector2f bot_pos = bot.board().get_position();
	
	float needed_xpos_player = window.getSize().x * 0.25f;
	float needed_xpos_bot = window.getSize().x * 0.75f;
	while (window.isOpen())
	{
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

		if (xpos_player - KOEF_SMOOTHNESS > needed_xpos_player) {
			xpos_player -= KOEF_SMOOTHNESS;
			player.board().setPosition(xpos_player,ypos_player);
		}
		else {
			player.board().setPosition(needed_xpos_player, ypos_player);
		}

		if (xpos_bot + KOEF_SMOOTHNESS < needed_xpos_bot) {
			xpos_bot += KOEF_SMOOTHNESS;
			bot.board().setPosition(xpos_bot, ypos_bot);
		}
		else {
			bot.board().setPosition(needed_xpos_bot, ypos_bot);
		}

		window.clear();
		//player.board().draw(window);

	}
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
// 7. Установить const для get-методов
// 
//