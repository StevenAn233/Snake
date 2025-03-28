// map.h
#pragma once
#include <iostream>
#include <string>

const int MAP_W {109};
const int MAP_H {30};
const int MAX_SIZE = MAP_W * MAP_H;
const std::string frame_str[15]
{
	"┌", "┐", "└", "┘", "╥", "╨", "─", "│", "║", 
	"┏", "┓", "┗", "┛", "━", "┃"
};

enum side {UPSIDE, DOWNSIDE};

class Map
{
private:
	static std::string info[MAP_H];
	static bool occupied[MAP_W][MAP_H];
	void putframe(int X, int Y, std::string type);
	void putrow(int space, int Y, side side);
	void putline();
public:
	void init();
	void clean();
	void printOver();
	void printStart();
	void printPAUSE()
	{goto_coord(MAP_W, MAP_H - 2); std::cout << " ●PAUSED";}
	void cleanPAUSE()
	{goto_coord(MAP_W, MAP_H - 2); std::cout << "        ";}
	void setOccupied(int x, int y, bool judge)
	{occupied[x][y] = judge;}
	static void goto_coord(int x, int y);
	static void printInfo(std::string name, int value, int width);
	static bool checkOccupied(int x, int y)
	{return occupied[x][y];}
};