#pragma once

#define BEIGE 245,245,220
#define DARK_BROWN 53,23,12
#define APRICOT 204, 120, 52

const int KOEF_SMOOTHNESS = 20; // ��� ������� ������� ����� �����, ������� �� ������� ������ ���������� �������

enum class DIRECTION
{
	//�������� ������������� ����� ������-�������
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	NON_DIR = 0
};

//enum class 