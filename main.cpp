#include "game.h"

Game game;

int main()
{
	game.init_map();
	while(true)
	{
		game.init_menu();
		if(!game.menu_anime()) break;
		game.init_round();
		game.gaming();
		if(!game.check_key()) break;
		else continue;
	}
	return 0;
}