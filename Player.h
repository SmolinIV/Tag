#pragma once
#include "Constants.h"
#include <string>
#include "Board.h"
class Player
{
private: 
	Board& p_board;
	float p_res_ftime;
	std::string p_res_strtime;
	int p_moves;

public:
	
	Player(Board &board);

	float get_ftime_res() const { return p_res_ftime; }
	const std::string& get_strtime_res() const { return p_res_strtime; }
	int get_moves() { return p_moves; }

	void reset_results();
	void set_fresult(float res) { p_res_ftime = res; };
	void set_strresult(std::string& res) { p_res_strtime = res; }
	void new_move() { ++p_moves; }
	Board& board() { return p_board; }
};

Player::Player(Board& board) :
	p_board{ board },
	p_res_ftime(0),
	p_res_strtime{ "00:00" },
	p_moves(0)
{}

void Player::reset_results() {
	p_res_ftime = 0;
	p_res_strtime = { "00:00" };
	p_moves = 0;
}