// food.h
#pragma once
#include <string>
#include "map.h"

class Food
{
private:
	int X{};
	int Y{};
	int score;
	int loop{};
	std::string shape; 
	static Food* beFood[MAP_W][MAP_H];    // For all food objects.
public:
	Food(std::string Shape, int Score)
	{
		shape = Shape;
		score = Score;
	}
	int getScore() const {return score;}
	void create();
	void cleanLoop() {loop = 0;}
	void create_as_snake_dies(int x, int y);
	void food_loop(int time, int term, int line = 0);
	static void setFood(int x, int y, Food* type);
	static Food* checkFood(int x, int y);
};

inline void Food::setFood(int x, int y, Food* type)
{
	beFood[x][y] = type;
}

inline Food* Food::checkFood(int x, int y)
{
	return beFood[x][y]; 
}