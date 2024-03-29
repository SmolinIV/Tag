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
	// �������� ��������� �� ������� � ��������
	float get_ftime_res() const { return p_res_ftime; }
	// �������� ��������� � ��������� ���� � ������� ��:��:��
	const std::string& get_strtime_res() const { return p_res_strtime; }
	//�������� ���������� ����� ������
	int get_moves() { return p_game_moves; }
	// �������� ��������� �� ������� � ��������
	void set_ftime_res(float res) { p_res_ftime = res; };
	// �������� ��������� �� ������� � ��������� ���� � ������� ��:��:��
	void set_strtime_res(std::string& res) { p_res_strtime = res; }
	// ��������� ����� ������
	void new_move() { ++p_game_moves; }
	// �������� ������ � �������� ���� ������
	Board& board() { return p_board; }
	// �������� ���������� ������
	void reset_results();

};
