#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include "map.h"
#include "food.h"
#include "snake.h"
using std::string;
using std::cout;

bool Map::occupied[MAP_W][MAP_H] {false};
string Map::info[MAP_H] {"\0"};

void Map::goto_coord(int x, int y)
{
	COORD coord{};
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Map::putframe(int X, int Y, std::string type)
{
	goto_coord(X, Y);
	std::cout << type;
	occupied[X][Y] = true;
}

void Map::putrow(int space, int Y, side side)
{
	string left, middle, right, straight;
	if(!space)
	{
		straight = frame_str[6];
		if(side == UPSIDE)
		{
			left = frame_str[0];
			middle = frame_str[4];
			right = frame_str[1];
		}
		else if(side == DOWNSIDE)
		{
			left = frame_str[2];
			middle = frame_str[5];
			right = frame_str[3];
		}
		putframe(0, Y, left);
		for(int i = 1; i < MAP_W - 1; i++)
			putframe(i, Y, straight);
		putframe(MAP_W - 1, Y, middle);
		for(int i = MAP_W; i < MAP_W + 9; i++)
			cout << straight;
		cout << right;
	}
	else
	{
		straight = frame_str[13];
		if(side == UPSIDE)
		{
			left = frame_str[9];
			right = frame_str[10];
		}
		else if(side == DOWNSIDE)
		{
			left = frame_str[11];
			right = frame_str[12];
		}
		putframe(space, Y, left);
		for(int i = (1 + space); i < MAP_W - (1 + space); i++)
			putframe(i, Y, straight);
		putframe(MAP_W - (1 + space), Y, right);
	}
}

void Map::putline()
{
	for(int j = 1; j < MAP_H - 1; j++)
	{
		putframe(0, j, frame_str[7]);
		putframe(MAP_W - 1, j, frame_str[8]);
	}
	for(int j = 2; j < MAP_H - 2; j++)
	{
		putframe(2, j, frame_str[14]);
		putframe(MAP_W - 3, j, frame_str[14]);
	}
	for(int j = 1; j < MAP_H - 1; j++)
	{
		goto_coord(MAP_W + 9, j);
		cout << frame_str[7];
		occupied[1][j] = true;
		occupied[MAP_W - 2][j] = true;
	}
}

void Map::init()
{
	putrow(0, 0, UPSIDE);
	putrow(0, MAP_H - 1, DOWNSIDE);
	putrow(2, 1, UPSIDE);
	putrow(2, MAP_H - 2, DOWNSIDE);
	putline();
}

void Map::clean()
{
	for(int y = 2; y < MAP_H - 2; y++)
	{
		goto_coord(4, y);
		std::cout.fill(' ');
		cout << std::setw(MAP_W - 7) << " ";
		for(int x = 4; x < MAP_W - 4; x += 2)
		{
			setOccupied(x, y, false);
			Food::setFood(x, y, nullptr);
			Snake::setSnake(x, y, nullptr);
		}
	}
	for(int i = 1; i < MAP_H - 1; i++)
	{
		goto_coord(MAP_W, i);
		cout << "         ";
		info[i] = "\0";
	}
}

void Map::printInfo(string name, int value, int width)
{
	int i{};
	for(i = 1; i < MAP_H; i += 2)
	{
		if(info[i] == "\0")
		{
			info[i] = name;
			break;
		}
		else if(info[i] == name)
			break;
	}
	goto_coord(MAP_W, i);
	cout << name;
	goto_coord(MAP_W, i + 1);
	cout.width(width);
	cout.fill('0');
	cout << value;
}

void Map::printOver()
{
	goto_coord(4, 2); std::cout << "╔═════╗               ╔═════╗              ";
	goto_coord(4, 3); std::cout << "║ ╔═╗ ║               ║ ╔═╗ ║              ";
	goto_coord(4, 4); std::cout << "║ ║╔╩═╬════╦═╗╔═╦════╗║ ║ ║ ╠═╗╔═╦════╦═══╗";
	goto_coord(4, 5); std::cout << "║ ║╚╗ ║ ╔╗ ║ ══ ║ ║══╣║ ║ ║ ║ ╚╝ ║ ║══╣ ╔═╝";
	goto_coord(4, 6); std::cout << "║ ╚═╝ ║ ╔╗ ║ ╠╣ ║ ║══╣║ ╚═╝ ╠╗  ╔╣ ║══╣ ║  ";
	goto_coord(4, 7); std::cout << "╚═════╩═╝╚═╩═╝╚═╩════╝╚═════╝╚══╝╚════╩═╝  ";
	goto_coord(4, 8); std::cout << "Press 'C' to continue. ";
	goto_coord(4, 9); std::cout << "Press 'X' to quit.     ";
	goto_coord(MAP_W + 10, MAP_H - 1);
}

void Map::printStart()
{
	for(int x = 30; x <= 30 + 48; x += 2)
		for(int y = 9; y <= 14; y++)
			setOccupied(x, y, true);
	for(int x = 30; x <= 30 + 18; x += 2)
	{
		setOccupied(x, 15, true);
		setOccupied(x, 16, true);
	}
	goto_coord(30,  9); std::cout << "╔═════╗         ╔═╗        ╔═════╗               ";
	goto_coord(30, 10); std::cout << "║ ╔═══╝         ║ ║        ║ ╔═╗ ║               ";
	goto_coord(30, 11); std::cout << "║ ╚═══╦═╗╔═╦════╣ ║╔═╦════╗║ ║╔╩═╬════╦═╗╔═╦════╗";
	goto_coord(30, 12); std::cout << "╚═══╗ ║ ╚╣ ║ ╔╗ ║ ══ ║ ║══╣║ ║╚╗ ║ ╔╗ ║ ══ ║ ║══╣";
	goto_coord(30, 13); std::cout << "╔═══╝ ║ ╠╗ ║ ╔╗ ║ ╠╗ ║ ║══╣║ ╚═╝ ║ ╔╗ ║ ╠╣ ║ ║══╣";
	goto_coord(30, 14); std::cout << "╚═════╩═╝╚═╩═╝╚═╩═╝╚═╩════╝╚═════╩═╝╚═╩═╝╚═╩════╝";
	goto_coord(30, 15); std::cout << "Press 'C' to start. ";
	goto_coord(30, 16); std::cout << "Press 'X' to quit.  ";
	goto_coord(MAP_W + 10, MAP_H - 1);
}