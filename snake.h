// snake.h
#pragma once
#include <string>
#include "food.h"
#include "map.h"

enum snake_sort{PLAYER, NPC};

class Snake
{
private:
	int len;
	int start_len;
	int speed;
	int start_speed;
	int len_value{};
	int score_total{};
	int pre_key{72};
	int X[MAX_SIZE]{};
	int Y[MAX_SIZE]{};
	int x_tail{};
	int y_tail{};
	unsigned int step_count{};
	const int UPSPEED{60};
	bool alive{false};
	bool did_speed_up{false};
	bool once_spawned{false};
	bool transformed{false};
	static Snake* beSnake[MAP_W][MAP_H];    // For all snakes.
	std::string head{"██"};
	std::string body{"██"};
	std::string name;
	Food* eaten_food {nullptr};
	snake_sort sort;
	void checkAlive();
	Food* ifEaten();
public:
	Snake(std::string Name,snake_sort Sort,
	int Start_len, int Start_speed = 120)
	{
		name = Name;
		start_len = Start_len;
		len = Start_len;
		start_speed = Start_speed;
		speed = Start_speed;
		sort = Sort;
	}
	void clean();
	void move(int key);
	void init(int x = (MAP_W / 2), int y = (MAP_H / 2));
	void init_random();
	void speed_up(bool judge);
	void setSpawned(bool judge) {once_spawned = judge;}
	void setTransformed(bool judge) {transformed = judge;}
	static void setSnake(int x, int y, Snake* type)
	{beSnake[x][y] = type;}
	int getSpeed() const {return speed;}
	int getLen() const {return len;}
	int getPre_key() const {return pre_key;}
	int getScore_total() const {return score_total;}
	int get_x_tail() const {return x_tail;}
	int get_y_tail() const {return y_tail;}
	int* getX() {int* x = X; return x;}
	int* getY() {int* y = Y; return y;}
	bool getAlive() const {return alive;}
	bool canSpeed_up() const {return len > 1;}
	bool onceSpawned() const {return once_spawned;}
	bool getTransformed() const {return transformed;}
	static Snake* checkSnake(int x, int y)
	{return beSnake[x][y];}
	Food* get_ifEaten() {return eaten_food;}
};