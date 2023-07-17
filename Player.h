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
	// Получить результат по времени в секундах
	float get_ftime_res() const { return p_res_ftime; }
	// Получить результат в строковом виде в формате ЧЧ:ММ:СС
	const std::string& get_strtime_res() const { return p_res_strtime; }
	//Получить количество ходов игрока
	int get_moves() { return p_game_moves; }
	// Изменить результат по времени в секундах
	void set_ftime_res(float res) { p_res_ftime = res; };
	// Изменить результат по времени в строковом виде в формате ЧЧ:ММ:СС
	void set_strtime_res(std::string& res) { p_res_strtime = res; }
	// Интератор ходов игрока
	void new_move() { ++p_game_moves; }
	// Получить доступ к игровому полю игрока
	Board& board() { return p_board; }
	// Сбросить результаты игрока
	void reset_results();

};
