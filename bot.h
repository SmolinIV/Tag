#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constants.h"

class Bot {
private:
	Board b_board;
	float b_res_ftime;
	std::string b_res_strtime;
	int b_moves;

	struct Cube_position { int i, j; } b_empty_cube, b_;

public:
	Bot();
	Bot(const Board& board);

	float get_ftime_res() const { return b_res_ftime; }
	const std::string& get_strtime_res() const { return b_res_strtime; }
	int get_moves() { return b_moves; }

	void reset_results();
	void set_fresult(float res) { b_res_ftime = res; };
	void set_strresult(std::string& res) { b_res_strtime = res; }
	void new_move() { ++b_moves; }
	Board& board() { return b_board; }
};

Bot::Bot() :
	b_res_ftime(0),
	b_res_strtime{ "00:00" },
	b_moves(0)
{}

Bot::Bot(const Board& board) : 
	Bot() {
	b_board = board;
}

void Bot::reset_results() {
	b_res_ftime = 0;
	b_res_strtime = { "00:00" };
	b_moves = 0;
}