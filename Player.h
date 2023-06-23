#pragma once
#include "Constants.h"
#include <string>
#include "Board.h"

class Player {
private: 
	Board& p_board;
	float p_res_ftime;
	std::string p_res_strtime;
	int p_game_moves;

public:
	Player();
	Player(Board &board);

	float get_ftime_res() const;
	const std::string& get_strtime_res() const;
	int get_moves();

	void reset_results();
	void set_ftime_res(float res);
	void set_strtime_res(std::string& res);
	void new_move();
	Board& board();
};
