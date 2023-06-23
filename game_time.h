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

