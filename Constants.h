#pragma once

#define RGB_BEIGE 245,245,220
#define RGB_DARK_BROWN 53,23,12
#define RGB_APRICOT 204, 120, 52
#define RGB_LIGHT_BROWN 222, 150, 1

int KOEF_SMOOTHNESS = 50; // Для эффекта отскока нужно число, которму не кратнен размер двигаемого объекта

enum class DIRECTION
{
	//Значения соответствуют кодам клавиш-стрелок
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	NON_DIR = 0
};

enum class PROCESS_STEPS
{
	MAIN_MENU,
	START_WITH_SHAFFLING,
	START_PLAYING,
	START_PLAING_WITH_BOT,
	GO_TO_OPTIONS,
	GO_TO_POSTGAME_MENU,
	EXIT,
	NONE
};

enum class GAMING_RESULT
{
	PLAYER_WIN,
	PLAYER_LOSE,
	PC_WIN,
	GAME_DRAW,
	GAME_PAUSE
};


//Задержка времени
//sf::Clock clock;
//float time = 0;
//clock.restart();
//while (time < 100) time = clock.getElapsedTime().asMilliseconds();
