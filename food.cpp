#include <iostream>
#include <string>
#include <cstdlib>
#include "map.h"
#include "snake.h"
#include "food.h"
using std::string;
using std::cout;

Food* Food::beFood[MAP_W][MAP_H] {nullptr};

void Food::create()
{
	while(true)
	{
		X = rand() % (MAP_W - 6) + 4;
		Y = rand() % (MAP_H - 3) + 2;
		if(X % 2 == 0 &&
		!Map::checkOccupied(X, Y) &&
		Snake::checkSnake(X, Y) == nullptr &&
		checkFood(X, Y) == nullptr)
		break;
	}
	Map::goto_coord(X, Y);
	cout << shape;
	setFood(X, Y, this);
}

void Food::create_as_snake_dies(int x, int y)
{
	Map::goto_coord(x, y);
	cout << shape;
	setFood(x, y, this);
}

void Food::food_loop(int time, int term, int line/*=0*/)
{
	loop += time;
	if(loop >= term)
	{
		loop -= term;
		create();
	}
}