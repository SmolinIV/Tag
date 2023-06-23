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

// Задание параметров текста, который будет выводиться на экран
void init_text(sf::Text& text, float xpos, float ypos, sf::String str, int size_font = 60,
	sf::Color menu_text_color = sf::Color(RGB_APRICOT), int bord = 3, sf::Color border_color = sf::Color(RGB_DARK_BROWN));

// Функции для различных этапов игры (соответствуют названиям)
PROCESS_STEPS main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error);
PROCESS_STEPS postgame_menu(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAME_RESULT& game_res);
PROCESS_STEPS shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error);
PROCESS_STEPS single_player(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAME_RESULT& game_res);
PROCESS_STEPS player_vs_bot(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAME_RESULT& game_res);

// Анимация "разъезда" игровых полей игрока и бота в разные стороны
void move_boards_apart(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, Bot& bot);

int main()
{
	std::error_code syst_error;
	do {
		srand(time(NULL));
		// Задание размеров окна (пока фиксированно, т.к. размеры объектов тоже заданы, а не привязаны к рамзеру окна
		float w_width = 1440,//sf::VideoMode::getDesktopMode().width/2,
			w_height = 800;	 //sf::VideoMode::getDesktopMode().height/2;

		// Название окна и иконка
		sf::RenderWindow window(sf::VideoMode(w_width, w_height), L"Пятнашки", sf::Style::Default);
		sf::Image icon;
		if (!icon.loadFromFile("png/m_cube_15.png")) { syst_error = std::make_error_code(std::errc::no_such_file_or_directory); break; }
		window.setIcon(32, 32, icon.getPixelsPtr());

		window.setFramerateLimit(60);
		window.setVerticalSyncEnabled(true);
		
		//Задний фон игры
		sf::RectangleShape background(sf::Vector2f(w_width, w_height));
		sf::Texture texture_background;
		if (!texture_background.loadFromFile("png/bg2.png")) { syst_error = std::make_error_code(std::errc::no_such_file_or_directory); break; }
		background.setTexture(&texture_background);

		Board player_board(syst_error);
		if (syst_error) { break; }

		Player player(player_board);

		// Алгоритм запускается с начального меню и далее все функции в качестве результата выполнения возвращают, какой этап запускать следующим.
		PROCESS_STEPS next_step = PROCESS_STEPS::MAIN_MENU; // Переменная-"указатель" на следующий этап
		PROCESS_STEPS last_game = PROCESS_STEPS::NONE;		// Переменная хранит тип последней игры для её повторного запуска
		GAME_RESULT game_res = GAME_RESULT::NONE;
		do {
			switch (next_step) {
			case PROCESS_STEPS::MAIN_MENU:
				next_step = main_menu(window, background, syst_error);
				last_game = next_step;
				break;
			case PROCESS_STEPS::SHAFFLING:
				next_step = shuffle_board(window, background, player.board(), syst_error);
				break;
			case PROCESS_STEPS::POSTGAME_MENU:
				next_step = postgame_menu(window, background, player, syst_error, game_res);
				if (next_step == PROCESS_STEPS::RESTART_GAME) {
					next_step = last_game;
				}
				break;
			case PROCESS_STEPS::SINGLE_PLAYER:
				next_step = shuffle_board(window, background, player.board(), syst_error);
				if (next_step != PROCESS_STEPS::SINGLE_PLAYER) {
					break;
				}
				next_step = single_player(window, background, player, syst_error, game_res);
				break;
			case PROCESS_STEPS::PLAYER_WITH_BOT:
				next_step = shuffle_board(window, background, player.board(), syst_error);
				if (next_step != PROCESS_STEPS::SINGLE_PLAYER) {
					break;
				}
				next_step = player_vs_bot(window, background, player, syst_error, game_res);
				break;
			case PROCESS_STEPS::EXIT:
				next_step = PROCESS_STEPS::EXIT;
				break;
			case PROCESS_STEPS::NONE:
			default:
				syst_error = std::make_error_code(std::errc::protocol_error);
				next_step = PROCESS_STEPS::EXIT;
				break;
			}
		} while (next_step != PROCESS_STEPS::EXIT);
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

// Главное меню с возможнотью перехода в одиночную игру и в "бой с компьютером", либо выход
PROCESS_STEPS main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error) {

	sf::Text title;
	sf::Font font;
	if (!font.loadFromFile("font/acsiomasupershockc.otf")) {
		syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
		return PROCESS_STEPS::EXIT;
	}
	title.setFont(font);
	init_text(title, window.getSize().x / 2, window.getSize().y / 6, L"Пятнашки", 100, sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));

	sf::Vector2i moving_pos{0, 0}, // Координаты курсора при считывании ... 
				 click_pos{ 0,0 }; // ... и при нажатии на ЛКМ

	Menu main_menu{ window,{L"Одиночная игра", L"Бой с компьютером", L"Выход"} };
	int menu_point_number = -1;		// Выбранный пункт меню (пока выбор не сделан - состояние -1)
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
			case sf::Event::MouseMoved:
				moving_pos = sf::Mouse::getPosition(window);
				main_menu.menu_navigating(moving_pos);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.key.code == sf::Mouse::Left) {
					click_pos = sf::Mouse::getPosition(window);
					menu_point_number = main_menu.click_point(click_pos);
				}
				break;
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
		case 0: return PROCESS_STEPS::SINGLE_PLAYER;
		case 1: return PROCESS_STEPS::PLAYER_WITH_BOT; 
		case 2: return PROCESS_STEPS::EXIT;
		default:
			break;
		}
	}
}
// Перемешивание колоды с анимацией процесса (для того, чтобы игрок видел, что последовательность не нарушалась, сборка возможна в любом случае)
PROCESS_STEPS shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error) {

	Game_time shuffle_time(syst_error);
	if (syst_error) { return PROCESS_STEPS::EXIT; }

	player_board.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	player_board.set_smoothness(koef_smoothness_shuffle); //Увеличиваем скорость перемещения кубиков

	sf::Text title,			// Надпись над полем
			 go_to_menu;	// Надпись под полем (ESC для возврата в гл.меню)
	sf::Font font;
	if (!font.loadFromFile("font/troika.otf")) {
		syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
		return PROCESS_STEPS::EXIT;
	}
	title.setFont(font);
	go_to_menu.setFont(font);
	init_text(title, window.getSize().x / 2, window.getSize().y * 0.09f, L"Приготовьтесь!", 80);
	init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"Для возврата в главное меню нажмите ESC", 20);

	bool is_moving = false;			// Флаг для функций перемещения кубиков (пока true, следующее перемещение не разрешшается)
	float time_for_shuffle = 4.0f;	// Время перемешивания
	while (window.isOpen() && shuffle_time.get_ftime() < time_for_shuffle) {
		shuffle_time.init_time();
		if (time_for_shuffle - shuffle_time.get_ftime() < 3.0f  && shuffle_time.get_ftime() < time_for_shuffle) {
			init_text(title, window.getSize().x / 2, window.getSize().y * 0.09f, std::to_string((int)(time_for_shuffle - shuffle_time.get_ftime()) + 1) + "...", 80);
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
		if (shuffle_time.get_ftime() < time_for_shuffle) {
			player_board.shaffle_board(window);
		}
		is_moving = true;
	}
	return PROCESS_STEPS::SINGLE_PLAYER;
}

//Одиночная игра с возможностью выйти в меню
PROCESS_STEPS single_player(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAME_RESULT& game_res) {
	do {
		player.reset_results();
		player.board().set_smoothness(koef_smoothness_player);
		sf::Text title, // Надпись над полем
			go_to_menu; // Надпись под полем (ESC для возврата в гл.меню)
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y * 0.09f, L"Начали!", 80);
		go_to_menu.setFont(font);
		init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"Для возврата в главное меню нажмите ESC", 20);

		Game_time running_time(syst_error);
		if (syst_error) { return PROCESS_STEPS::EXIT; }
		running_time.set_position(window.getSize().x - 200, 30);

		bool is_moving = false; // Флаг для функций перемещения кубиков(пока true, следующее перемещение не разрешшается)
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
						game_res = GAME_RESULT::PLAYER_GAVE_UP;
						player.set_ftime_res(running_time.get_ftime());
						player.set_strtime_res(running_time.get_strtime());
						return PROCESS_STEPS::POSTGAME_MENU;
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
				game_res = GAME_RESULT::PLAYER_WIN;
				player.set_ftime_res(running_time.get_ftime());
				player.set_strtime_res(running_time.get_strtime());
				return PROCESS_STEPS::POSTGAME_MENU;
			}
		}
	} while (false);
	return PROCESS_STEPS::EXIT;
}

// Послеигровое меню (запусается при победе/поражении/сдаче) с возможностью повторной игры, либо выхода в гл.меню
PROCESS_STEPS postgame_menu(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAME_RESULT& game_res) {

		//Шрифт для результата игры
		sf::Text result, // Результат игры
			time_res;	 // Затраченное игроком время
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) { 
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT; }

		result.setFont(font);
		time_res.setFont(font);

		sf::String res_message; 
		switch (game_res)
		{
		case GAME_RESULT::PLAYER_WIN:
			res_message = L"Вы победили!";
			break;
		case GAME_RESULT::PC_WIN:
			res_message = L"Компьютер победил!";
			break;
		case GAME_RESULT::PLAYER_GAVE_UP:
			res_message = L"Попробуйте ещё раз!";
			break;
		default:
			break;
		}	

		init_text(result, window.getSize().x / 2, window.getSize().y / 7, res_message, 80, sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));
		if (game_res != GAME_RESULT::PLAYER_GAVE_UP) {
			init_text(time_res, window.getSize().x / 2, window.getSize().y / 4, L"Затраченное время: " + (sf::String)player.get_strtime_res(), 40,
				sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));
		}

		Menu main_menu{ window,{L"Сыграть еще раз", L"На главную", L"Выход"} };

		sf::Vector2i moving_pos{0, 0}, // Координаты курсора при считывании ... 
					 click_pos{ 0,0 }; // ... и при нажатии на ЛКМ

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
				case sf::Event::MouseMoved:
					moving_pos = sf::Mouse::getPosition(window);
					main_menu.menu_navigating(moving_pos);
					break;
				case sf::Event::MouseButtonPressed:
					if (event.key.code == sf::Mouse::Left) {
						click_pos = sf::Mouse::getPosition(window);
						menu_point_number = main_menu.click_point(click_pos);
					}
					break;

				default:
					break;
				}
			}
			window.clear();
			window.draw(background);
			window.draw(result);
			window.draw(time_res);
			main_menu.draw(window);
			window.display();
			switch (menu_point_number) {
			case 0: return PROCESS_STEPS::RESTART_GAME;
			case 1: return PROCESS_STEPS::MAIN_MENU;
			case 2: return PROCESS_STEPS::EXIT;
			default:
				break;
			}
		}
}

// Игра против компютера
PROCESS_STEPS player_vs_bot(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, GAME_RESULT& game_res) {
	Bot bot{ player.board() };
	bot.board() = player.board();

	player.reset_results();
	bot.reset_results();
	move_boards_apart(window, background, player, bot);
	bot.set_start_pos();

	player.board().set_smoothness(koef_smoothness_player);
	bot.board().set_smoothness(koef_smoothness_bot);

	do {
		sf::Text title, go_to_menu, player_title, bot_title;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT;
		}
		title.setFont(font);
		player_title.setFont(font);
		bot_title.setFont(font);
		go_to_menu.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y * 0.09f, L"Начали!", 80);
		init_text(player_title, window.getSize().x *0.25f, window.getSize().y * 0.17f, L"Вы", 50);
		init_text(bot_title, window.getSize().x * 0.75f, window.getSize().y * 0.17f, L"Компьютер", 50);
		init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"Для возврата в главное меню нажмите ESC", 20);

		
		Game_time running_time(syst_error);
		if (syst_error) { return PROCESS_STEPS::EXIT; }
		running_time.set_position(window.getSize().x - 200, 30);

		bool is_moving = false;  // Флаг для функций перемещения кубиков(пока true, следующее перемещение не разрешшается) (у бота реализовано внутри класса)
		while (window.isOpen())
		{
			// Обработка действий
			sf::Event event;
			while (window.pollEvent(event)) {
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
						game_res = GAME_RESULT::PLAYER_GAVE_UP;
						player.set_ftime_res(running_time.get_ftime());
						player.set_strtime_res(running_time.get_strtime());
						return PROCESS_STEPS::POSTGAME_MENU;
					}
				default:
					break;
				}
			}
			do {
				bot.assemble_board(window);
				window.clear();
				window.draw(background);
				window.draw(title);
				window.draw(player_title);
				window.draw(bot_title);
				running_time.draw(window);
				player.board().draw(window, is_moving);	//is_moving обнуляется внутри функции
				bot.board().draw(window, *bot.get_permit_to_move());
				window.draw(go_to_menu);
				window.display();
			} while (is_moving);
			if (bot.assemble_done() && bot.get_ftime_res() < 1) { // Меньше 1, а не ==0, чтобы избежать проблем со сравнением чисел с плавающей запятой
				bot.set_ftime_res(running_time.get_ftime());
			}
			if (player.board().sequence_restored()) { // Ничья исключена :)
				if (bot.assemble_done()) {
					game_res = GAME_RESULT::PC_WIN;
				}
				else {
					game_res = GAME_RESULT::PLAYER_WIN;
				}
				player.set_ftime_res(running_time.get_ftime());
				player.set_strtime_res(running_time.get_strtime());
				return PROCESS_STEPS::POSTGAME_MENU;
			}	
		}
	} while (false);
	return PROCESS_STEPS::EXIT;
}

// Анимация разъезжающихся полей игрока и компьбтера
void move_boards_apart(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, Bot& bot) {

	
	float xpos_player = window.getSize().x / 2;
	float xpos_bot = xpos_player;
	float ypos_player = window.getSize().y / 2;
	float ypos_bot = ypos_player;
	
	float needed_xpos_player = window.getSize().x * 0.25f;
	float needed_xpos_bot = window.getSize().x * 0.75f;
	bool bot_board_in_place = false;
	bool player_board_in_place = false;
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

		if (xpos_player - koef_smoothness_board > needed_xpos_player) {
			xpos_player -= koef_smoothness_board;
			player.board().setPosition(xpos_player,ypos_player);
		}
		else {
			player.board().setPosition(needed_xpos_player, ypos_player); 
			player_board_in_place = true;
		}

		if (xpos_bot + koef_smoothness_board < needed_xpos_bot) {
			xpos_bot += koef_smoothness_board;
			bot.board().setPosition(xpos_bot, ypos_bot);
		}
		else {
			bot.board().setPosition(needed_xpos_bot, ypos_bot);
			bot_board_in_place = true;
		}

		window.clear();
		window.draw(background);
		bot.board().draw(window);
		player.board().draw(window);
		window.display();
		if (player_board_in_place && bot_board_in_place) { break; }
	}
}

// Задание параметров текста, который будет выводиться на экран (используются почти в каждой функции, поэтому выделено в отдельную функцию)
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
// 9. assemble_row_exept_last_cube переделать под "шаговый" алгоритм
//