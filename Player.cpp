#pragma once
#include "Player.h"

Player::Player(Board& board) :
	p_board{ board },
	p_res_ftime(0),
	p_res_strtime{ "00:00" },
	p_game_moves(0)
{}


float Player::get_ftime_res() const { return p_res_ftime; }
const std::string& Player::get_strtime_res() const { return p_res_strtime; }
int Player::get_moves() { return p_game_moves; }

void Player::set_ftime_res(float res) { p_res_ftime = res; };
void Player::set_strtime_res(std::string& res) { p_res_strtime = res; }
void Player::new_move() { ++p_game_moves; }
Board& Player::board() { return p_board; }


void Player::reset_results() {
	p_res_ftime = 0;
	p_res_strtime = { "00:00" };
	p_game_moves = 0;
}