#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Game_time {
private:
	float gt_ftime;
	int gt_iseconds;
	int gt_iminutes;
	int gt_ihours;
	sf::Clock gt_clock;
	sf::Text gt_time_in_text;
	sf::Font gt_font;
	std::string gt_strtime;

	sf::Vector2f gt_position{0, 0};

public:
	Game_time(std::error_code& syst_error);

	void restart() { gt_clock.restart(); }

	float get_ftime() { return gt_ftime; };
	
	std::string& const get_strtime() { return gt_strtime; }

	void init_time();

	void set_position(float xpos, float ypos);

	void draw(sf::RenderWindow& window);
	void init_text();
	//~Game_time();
};

Game_time::Game_time(std::error_code& syst_error) {
	do {
		restart();
		gt_ftime = gt_clock.getElapsedTime().asSeconds();
		if (!gt_font.loadFromFile("font/troika.otf")) {
			syst_error = std::make_error_code(std::errc::no_such_file_or_directory);
			break;
		}
		gt_time_in_text.setFont(gt_font);
		gt_strtime = "00:00";
		init_time();
		init_text();

		/*gt_time_in_text.setCharacterSize(60);
		gt_time_in_text.setString("00:00");
		gt_time_in_text.setFillColor(sf::Color(RGB_APRICOT));
		gt_time_in_text.setOutlineThickness(3);
		gt_time_in_text.setOutlineColor(sf::Color(RGB_DARK_BROWN));
		gt_time_in_text.setPosition(gt_position);*/
	} while (false);
}

void Game_time::init_time() {
	gt_ftime = gt_clock.getElapsedTime().asSeconds();
	gt_ihours = (int)gt_ftime / 3600;
	gt_iminutes = ((int)gt_ftime % 3600) / 60;
	gt_iseconds = (int)gt_ftime % 60;
}

void Game_time::set_position(float xpos, float ypos) {
	gt_position = { xpos, ypos };
	gt_time_in_text.setPosition(gt_position);
}

void Game_time::draw(sf::RenderWindow& window) {
	init_time();
	gt_strtime.clear();

	if (gt_ihours != 0) {
		gt_strtime = (gt_ihours < 10 ? "0" : "") + std::to_string(gt_ihours) + ":";
	}

	gt_strtime += (gt_iminutes < 10 ? "0" : "") + std::to_string(gt_iminutes) + ":" + (gt_iseconds < 10 ? "0" : "") + std::to_string(gt_iseconds);

	gt_time_in_text.setString(gt_strtime); //init_text();

	window.draw(gt_time_in_text);

}

void Game_time::init_text() {
	gt_time_in_text.setCharacterSize(60);
	gt_time_in_text.setString(gt_strtime);
	gt_time_in_text.setFillColor(sf::Color(RGB_APRICOT));
	gt_time_in_text.setOutlineThickness(3);
	gt_time_in_text.setOutlineColor(sf::Color(RGB_DARK_BROWN));
	gt_time_in_text.setPosition(gt_position);
}