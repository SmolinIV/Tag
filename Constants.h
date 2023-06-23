#pragma once

#define RGB_BEIGE 245,245,220
#define RGB_DARK_BROWN 53,23,12
#define RGB_APRICOT 204, 120, 52
#define RGB_LIGHT_BROWN 222, 150, 1


// Глобальные для удобства отладки, потом можно от них избавиться
float koef_smoothness_shuffle = 50.0f,
koef_smoothness_board = 5.0f,
koef_smoothness_player = 20.0f,
koef_smoothness_bot = 10.0f;


enum class DIRECTION {
	//Значения соответствуют кодам клавиш-стрелок
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	NON_DIR = 0
};

enum class PROCESS_STEPS {
	MAIN_MENU,
	SHAFFLING,
	SINGLE_PLAYER,
	PLAYER_WITH_BOT,
	POSTGAME_MENU,
	RESTART_GAME,
	EXIT,
	NONE
};

enum class GAME_RESULT {
	PLAYER_WIN,
	PC_WIN,
	PLAYER_GAVE_UP,
	NONE
};
