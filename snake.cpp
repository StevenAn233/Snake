#include <iostream>
#include <string>
#include "map.h"
#include "food.h"
#include "snake.h"
#include "game.h"
using std::cout;
using std::string;

Snake* Snake::beSnake[MAP_W][MAP_H] {nullptr};

void Snake::init(int x, int y)
{
	score_total = 0;
	alive = true;
	len = start_len;
	len_value += (len - 1) * 500;
	len = 1;
	X[0] = x;
	Y[0] = y;
	setSnake(X[0], Y[0], this);
	Map::goto_coord(X[0], Y[0]);
	cout << head;
	if(sort == PLAYER)
		Map::printInfo(name, score_total, 9);
}

void Snake::init_random()
{
	int X{}, Y{};
	while(true)
	{
		X = rand() % (MAP_W - 6) + 4;
		Y = rand() % (MAP_H - 3) + 2;
		if(X % 2 == 0 &&
		!Map::checkOccupied(X, Y) &&
		Snake::checkSnake(X, Y) == nullptr &&
		Food::checkFood(X, Y) == nullptr)
		break;
	}
	init(X, Y);
}

void Snake::clean()
{
	alive = false;
	once_spawned = false;
	transformed = false;
}

void Snake::checkAlive()
{
	if(Map::checkOccupied(X[0], Y[0])
	|| checkSnake(X[0], Y[0]) != nullptr)
		alive = false;
}

void Snake::speed_up(bool do_speed_up)
{
	did_speed_up = do_speed_up;
	speed = do_speed_up ? UPSPEED : start_speed;
}

void Snake::move(int key)
{
	pre_key = key;
	x_tail = X[len - 1]; y_tail = Y[len - 1];
	// Move the coordinate of the snake.
	for (int i = len - 1; i > 0; i--)
	{
		X[i] = X[i - 1];
		Y[i] = Y[i - 1];
	}
	switch(key)
	{
	case UP:
		Y[0]--;
		checkAlive();
		if(!alive) Y[0]++;
		break;
	case DOWN:
		Y[0]++;
		checkAlive();
		if(!alive) Y[0]--;
		break;
	case LEFT:
		X[0] -= 2; 
		checkAlive();
		if(!alive) X[0] += 2;
		break;
	case RIGHT:
		X[0] += 2; 
		checkAlive();
		if(!alive) X[0] -= 2;
		break;
	default: break;
	}
	if(!alive) return;
	Map::goto_coord(X[0], Y[0]);
	cout << head;
	eaten_food = ifEaten();
	setSnake(X[0], Y[0], this);
	if(!canSpeed_up()) did_speed_up = false;
	if(did_speed_up)
	{
		step_count++;
		if(step_count >= 12)
		{
			len = std::max(1, len - 1);
			step_count = 0;
			Map::goto_coord(X[len], Y[len]);
			cout << "  ";
			setSnake(X[len], Y[len], nullptr);
		}
	}
}

Food* Snake::ifEaten()
{
	Food* type = Food::checkFood(X[0], Y[0]);
	if(type != nullptr)
	{
		score_total += type->getScore();
		len_value += type->getScore();
		if(sort == PLAYER)
			Map::printInfo(name, score_total, 9);
	}
	if(len_value / 500 != 0)
	{
		len_value -= 500;
		Food::setFood(X[0], Y[0], nullptr);
		len++;
		X[len - 1] = x_tail;
		Y[len - 1] = y_tail;
		return type;
	}
	else
	{
		Map::goto_coord(x_tail, y_tail);
		cout << "  ";
		setSnake(x_tail, y_tail, nullptr);
		return nullptr;
	}
}