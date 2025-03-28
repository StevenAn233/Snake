// game.h
#pragma once
#include "snake.h"

enum key {UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77};

class Game
{
private:
	bool paused{false};
	int key_input(Snake& snake);
	int key_auto(Snake& snake);
	bool move_snake(Snake& snake, int key);
	void game_loop();
	void npc_action(Snake& snake, int total_time, int gap_time);
	void store_score(int score);	
	void transform(Snake* snake, Food* food);
public:
	void gaming();
	void init_map();
	void init_menu();
	void init_round();
	bool menu_anime();
	bool check_key();
};