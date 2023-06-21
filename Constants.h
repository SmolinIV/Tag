#pragma once

#define RGB_BEIGE 245,245,220
#define RGB_DARK_BROWN 53,23,12
#define RGB_APRICOT 204, 120, 52
#define RGB_LIGHT_BROWN 222, 150, 1

int KOEF_SMOOTHNESS = 50; // ��� ������� ������� ����� �����, ������� �� ������� ������ ���������� �������

enum class DIRECTION
{
	//�������� ������������� ����� ������-�������
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	NON_DIR = 0
};

enum class PROCESS_STEPS
{
	GO_TO_MAIN_MENU,
	START_SHAFFLING,
	START_PLAYING,
	START_PLAYING_WITH_BOT,
	GO_TO_POSTGAME_MENU,
	RESTART_GAME,
	EXIT,
	NONE
};


//�������� �������
//sf::Clock clock;
//float time = 0;
//clock.restart();
//while (time < 100) time = clock.getElapsedTime().asMilliseconds();
