#pragma once
#include "Player.h"

Player::Player(Board& board) :
	p_board{ board },
	p_res_ftime(0),
	p_res_strtime{ "00:00" },
	p_game_moves(0)
{}

void Player::reset_results() {
	p_res_ftime = 0;
	p_res_strtime = { "00:00" };
	p_game_moves = 0;
}