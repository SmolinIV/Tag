//��. ���������� � ��������� � �����
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

PROCESS_STEPS main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error);
PROCESS_STEPS postgame_menu(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, bool& player_win);
PROCESS_STEPS shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error);
PROCESS_STEPS single_player(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, bool& player_win);
PROCESS_STEPS player_vs_bot(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, bool& player_win);

void move_boards_apart(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, Bot& bot);

int main()
{
	setlocale(LC_ALL, "RUS");
	std::error_code syst_error;
	do {
		srand(time(NULL));
		// ������� �������� ����
		float w_width = 1440,//sf::VideoMode::getDesktopMode().width/2,
			w_height = 800;	 //sf::VideoMode::getDesktopMode().height/2;

		sf::RenderWindow window(sf::VideoMode(w_width, w_height), L"��������", sf::Style::Default);

		window.setFramerateLimit(60);
		window.setVerticalSyncEnabled(true);
		//window.setMouseCursorVisible(false);
		
		//������ ��� ����
		sf::RectangleShape background(sf::Vector2f(w_width, w_height));
		sf::Texture texture_background;
		if (!texture_background.loadFromFile("png/bg2.png")) { syst_error = std::make_error_code(std::errc::no_such_file_or_directory); break; }
		background.setTexture(&texture_background);

		Board player_board(syst_error);
		if (syst_error) { break; }

		Player player(player_board);

		PROCESS_STEPS next_step = PROCESS_STEPS::GO_TO_MAIN_MENU;
		PROCESS_STEPS last_game = PROCESS_STEPS::NONE;
		bool player_win = false;
		do {
			switch (next_step)
			{
			case PROCESS_STEPS::GO_TO_MAIN_MENU:
				next_step = main_menu(window, background, syst_error);
				last_game = next_step;
				break;

			case PROCESS_STEPS::START_SHAFFLING:
				next_step = shuffle_board(window, background, player.board(), syst_error);
				break;

			case PROCESS_STEPS::GO_TO_POSTGAME_MENU:
				next_step = postgame_menu(window, background, player, syst_error, player_win);
				if (next_step == PROCESS_STEPS::RESTART_GAME) {
					next_step = last_game;
				}
				break;

			case PROCESS_STEPS::START_PLAYING:
				next_step = shuffle_board(window, background, player.board(), syst_error);
				if (next_step != PROCESS_STEPS::START_PLAYING) {
					break;
				}
				next_step = single_player(window, background, player, syst_error, player_win);
				break;

			case PROCESS_STEPS::START_PLAYING_WITH_BOT:
				next_step = shuffle_board(window, background, player.board(), syst_error);
				if (next_step != PROCESS_STEPS::START_PLAYING) {
					break;
				}
				next_step = player_vs_bot(window, background, player, syst_error, player_win);
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

PROCESS_STEPS main_menu(sf::RenderWindow& window, sf::RectangleShape& background, std::error_code& syst_error) {

	//����� ��� �������� ����
	sf::Text title;
	sf::Font font;
	if (!font.loadFromFile("font/acsiomasupershockc.otf")) {
		syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
		return PROCESS_STEPS::EXIT;
	}
	title.setFont(font);
	init_text(title, window.getSize().x / 2, window.getSize().y / 6, L"��������", 100, sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));

	Menu main_menu{ window,{L"��������� ����", L"��� � �����������", L"�����"} };
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
		case 0: return PROCESS_STEPS::START_PLAYING;
		case 1: return PROCESS_STEPS::START_PLAYING_WITH_BOT; 
		case 2: return PROCESS_STEPS::EXIT;
		default:
			break;
		}
	}
}

PROCESS_STEPS shuffle_board(sf::RenderWindow& window, sf::RectangleShape& background, Board& player_board, std::error_code& syst_error) {

	KOEF_SMOOTHNESS = 50;
	player_board.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	sf::Text title, go_to_menu;
	sf::Font font;
	if (!font.loadFromFile("font/troika.otf")) {
		syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
		return PROCESS_STEPS::EXIT;
	}
	title.setFont(font);
	init_text(title, window.getSize().x / 2, window.getSize().y * 0.09f, L"�������������!", 80);

	go_to_menu.setFont(font);
	init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"��� �������� � ������� ���� ������� ESC", 20);
	bool is_moving = false;
	int shuffle_steps = 0;
	int number_of_swap = 60;
	while (window.isOpen() && shuffle_steps < number_of_swap) {
		if (shuffle_steps >= number_of_swap / 2 && (number_of_swap - shuffle_steps) % 10 == 0) {
			init_text(title, window.getSize().x / 2, window.getSize().y * 0.09f, std::to_string((number_of_swap - shuffle_steps) / 10) + "...", 80);
		}

		// ��������� ��������
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
					return PROCESS_STEPS::GO_TO_MAIN_MENU; }
			default:
				break;
			}
		}
		do {
			window.clear();
			window.draw(background);
			window.draw(title);
			window.draw(go_to_menu);
			player_board.draw(window, is_moving);	//is_moving ���������� ������ �������
			window.display();
		} while (is_moving);
		++shuffle_steps;
		if (shuffle_steps < number_of_swap) {
			player_board.shaffle_board(window);
		}
		is_moving = true;
	}
	KOEF_SMOOTHNESS = 10;
	return PROCESS_STEPS::START_PLAYING;
}

PROCESS_STEPS single_player(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, bool& player_win) {
	player.reset_results();
	do {
		sf::Text title, go_to_menu;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y / 8, L"������!", 80);
		go_to_menu.setFont(font);
		init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"��� �������� � ������� ���� ������� ESC", 20);

		Game_time running_time(syst_error);
		if (syst_error) { return PROCESS_STEPS::EXIT; }
		running_time.set_position(window.getSize().x - 200, 30);

		bool is_moving = false;
		while (window.isOpen())
		{
			// ��������� ��������
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
						return PROCESS_STEPS::GO_TO_MAIN_MENU;
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
				player.board().draw(window, is_moving);	//is_moving ���������� ������ �������
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

PROCESS_STEPS postgame_menu(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, bool& player_win) {

	/*	sf::RectangleShape deck(sf::Vector2f(500, 500));
		deck.setPosition(window.getSize().x / 2 - deck.getGlobalBounds().width/2, window.getSize().y / 2 - deck.getGlobalBounds().height / 2);
		sf::Texture deck_texture;
		if (!deck_texture.loadFromFile("png/ended_deck1.png")) { 
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory); 
			return PROCESS_STEPS::EXIT;
		}
		deck.setTexture(&deck_texture);*/

		//����� ��� ���������� ����
		sf::Text result, time_res;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) { 
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT; }

		result.setFont(font);
		time_res.setFont(font);
		
		init_text(result, window.getSize().x / 2, window.getSize().y / 7, L"������!", 100, sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));
		init_text(time_res, window.getSize().x / 2, window.getSize().y / 4, L"����� ����������: " + (sf::String)player.get_strtime_res(), 50,
			      sf::Color(RGB_APRICOT), 3, sf::Color(RGB_DARK_BROWN));

		Menu main_menu{ window,{L"������� ��� ���", L"� ������� ����", L"�����"} };

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
			case 0: return PROCESS_STEPS::START_PLAYING;
			case 1: return PROCESS_STEPS::GO_TO_MAIN_MENU;
			case 2: return PROCESS_STEPS::EXIT;
			default:
				break;
			}
		}
}

PROCESS_STEPS player_vs_bot(sf::RenderWindow& window, sf::RectangleShape& background, Player& player, std::error_code& syst_error, bool& player_win) {
	Bot bot{ player.board() };
	bot.board() = player.board();

	player.reset_results();
	bot.reset_results();
	move_boards_apart(window, background, player, bot);
	bot.set_start_pos();
	do {
		sf::Text title, go_to_menu, player_title, bot_title;
		sf::Font font;
		if (!font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			return PROCESS_STEPS::EXIT;
		}
		title.setFont(font);
		init_text(title, window.getSize().x / 2, window.getSize().y * 0.09f, L"������!", 80);
		go_to_menu.setFont(font);
		init_text(go_to_menu, window.getSize().x / 2, window.getSize().y * 0.81f, L"��� �������� � ������� ���� ������� ESC", 20);

		player_title.setFont(font);
		init_text(player_title, window.getSize().x *0.25f, window.getSize().y * 0.17f, L"��", 50);
		bot_title.setFont(font);
		init_text(bot_title, window.getSize().x * 0.75f, window.getSize().y * 0.17f, L"���������", 50);
		
		

		Game_time running_time(syst_error);
		if (syst_error) { return PROCESS_STEPS::EXIT; }
		running_time.set_position(window.getSize().x - 200, 30);

		bool is_moving = false;
		while (window.isOpen())
		{
			// ��������� ��������
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
						res = GAMING_RESULT::PLAYER_LOSE;
						return PROCESS_STEPS::GO_TO_MAIN_MENU;
					}
				default:
					break;
				}
			}
				//if (event.key.code == sf::Mouse::Left || !bot.assemble_done()) {
					do {
						bot.assemble_board(window);
						window.clear();
						window.draw(background);
						window.draw(title);
						window.draw(player_title);
						window.draw(bot_title);
						running_time.draw(window);
						player.board().draw(window, is_moving);	//is_moving ���������� ������ �������
						bot.board().draw(window, *bot.get_permit_to_move());
						window.draw(go_to_menu);
						window.display();
					} while (is_moving);
					if (player.board().sequence_restored()) {
						res = GAMING_RESULT::PC_WIN;
						player.set_fresult(running_time.get_ftime());
						player.set_strresult(running_time.get_strtime());
						return PROCESS_STEPS::GO_TO_POSTGAME_MENU;
					}
				//}
			
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

		if (xpos_player - KOEF_SMOOTHNESS > needed_xpos_player) {
			xpos_player -= KOEF_SMOOTHNESS;
			player.board().setPosition(xpos_player,ypos_player);
		}
		else {
			player.board().setPosition(needed_xpos_player, ypos_player); 
			player_board_in_place = true;
		}

		if (xpos_bot + KOEF_SMOOTHNESS < needed_xpos_bot) {
			xpos_bot += KOEF_SMOOTHNESS;
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



//����������: ������������ ����������� �� ������������, �.�. ���������� ���, ��� ����� ������� �� ���������, �.�. ��� ������������� ��������� ������
// 
// ���������.
// 1. �� ������ ������������� �������� ��������� ��������
// 2. �����.
// 3. �������� ����� �������� ���������� � ��������� �������.
// 4. ������ std::array �� board, �� ������ ����������� �� ������������, ������ ��������� ���.
// 5. ����������� �������� ��������� ��� Cube.
// 6. ��������� ������ ������� � ����� (�� �������� ������ � �������/���������), � ��� �� ������� �������
// 7. ���������� const ��� get-�������
// 8. ����� ��������� (���� ���:) ). ������ ��� - ����� ��������� ���� ��� ������ ����������
// 9. assemble_row_exept_last_cube ���������� ��� "�������" ��������
//