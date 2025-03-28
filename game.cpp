#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "map.h"
#include "food.h"
#include "snake.h"
#include "game.h"
using std::cout;

Map map;
Food food1("▗▖", 500);
Food food2("✦ ", 1000);
Food food3("■ ", 250);
Snake player("PLAYER", PLAYER, 3);
Snake npc1("NPC1", NPC, 5);

const int LOOPTIME{30};
const int GAPTIME{6000};
const int SPAWNTIME1{18000};

void Game::init_map() 
{
    cout << "\033[?25l";
    SetConsoleOutputCP(CP_UTF8);
    map.init();
}

void Game::init_menu()
{
    map.clean();
    Sleep(100);
    int score{};
    std::string line;
    std::ifstream fin("storage.md");
    std::getline(fin, line);
    fin.close();
    int best_score = atoi(line.c_str());
    Map::printInfo("BESTSCORE", best_score, 9);
    map.printStart();

}

void Game::init_round()
{
    map.clean();
    Sleep(100);
    player.init();
    npc1.clean();
    food1.cleanLoop();
    food2.cleanLoop();
    food1.create();
}

bool Game::check_key()
{
    char ch{};
    while(ch = _getch())
    {
        if(ch == 'c') return true;
        else if(ch == 'x') return false;
        else continue;
    }
    return false;
}

void Game::game_loop()
{
	food1.food_loop(LOOPTIME, 4800);
	food2.food_loop(LOOPTIME, 17400);
	Sleep(LOOPTIME);
}

bool Game::move_snake(Snake& snake, int key)
{
	if(!snake.getAlive())
        return false;
	snake.move(key);
	return true;
}

void Game::npc_action(Snake& snake, int total_time)
{
    if((total_time % snake.getSpeed()) == 0)
	{
		static int npc_time = 0;
		if(!move_snake(snake, key_auto(npc1))
        && snake.onceSpawned())
		{
			npc_time += snake.getSpeed();
			if(!snake.getTransformed())
			{
				transform(&snake, &food3);
				snake.setTransformed(true);
			}
		}
		if(npc_time >= GAPTIME && snake.getTransformed())
		{
			npc_time = 0;
			snake.init_random();
			snake.setTransformed(false);
		}
	}
	if(!snake.onceSpawned() && total_time >= SPAWNTIME1)
	{
		snake.setSpawned(true);
		snake.init_random();
	}
}

void Game::store_score(int score)
{
    std::string line;
    std::ifstream fin("storage.md");
    std::getline(fin, line);
    fin.close();
    int max_score = atoi(line.c_str());
    if(score > max_score)
    {
        std::ofstream fout("storage.md");
        fout << score;
        fout.close();
    }
}

void Game::gaming()
{
	srand((unsigned int)time(NULL));
	unsigned int total_time = 0;
	bool spawned_npc1 = false;
	bool transformed_npc1 = false;
	while(true)
	{
        if(paused)
        {
            key_input(player);
            Sleep(LOOPTIME);
            continue;
        }
		total_time += LOOPTIME;
		if((total_time % player.getSpeed()) == 0)
            if(!move_snake(player, key_input(player)))
            {
                store_score(player.getScore_total());
                map.printOver();
                break;
            }
		npc_action(npc1, total_time);
        game_loop();
	}
}

int Game::key_input(Snake& snake)
{
    bool shift_pressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000);
    snake.speed_up(shift_pressed && snake.canSpeed_up());
    int this_key {};
    int pre_key = snake.getPre_key();
	if(_kbhit())
	{
        fflush(stdin);
        int ch1 = _getch();
        if(ch1 == 0 || ch1 == 224)  // Direction keys.
        {
            int ch2 = _getch();  // Get the second char.
            switch(ch2)
            {
            case UP: this_key = UP; break;
            case DOWN: this_key = DOWN; break;
            case LEFT: this_key = LEFT; break;
            case RIGHT: this_key = RIGHT; break;
            default: break;
            }
        }
        else  // Normal keys.
        {
            // Supporting 'w'.'a'.'s'.'d'.
            switch(ch1)
            {
            case 'w': case 'W': this_key = UP; break;
            case 's': case 'S': this_key = DOWN; break;
            case 'a': case 'A': this_key = LEFT; break;
            case 'd': case 'D': this_key = RIGHT; break;
            case 'p': case 'P':
                if(!paused) 
                {paused = true; map.printPAUSE();}
                else 
                {paused = false; map.cleanPAUSE();}
                break;
            default: break;
            }
        }
    }
	// Direction can't be exactly opposite.
    if(snake.getLen() > 1)
    {
        if(pre_key == UP && this_key == DOWN)	this_key = UP;
        if(pre_key == DOWN && this_key == UP)	this_key = DOWN;
        if(pre_key == LEFT && this_key == RIGHT) this_key = LEFT;
        if(pre_key == RIGHT && this_key == LEFT) this_key = RIGHT;
    }
    return this_key ? this_key : pre_key;
}

int Game::key_auto(Snake& snake)  // Deepseek.
{
    int x0 = snake.getX()[0];
    int y0 = snake.getY()[0];
    // Find the nearest food.
    int min_distance = INT_MAX;
    int fx = -1, fy = -1;
    for(int x = 4; x < MAP_W - 4; x += 2)
        for(int y = 2; y < MAP_H - 2; y++)
            if(Food::checkFood(x, y) != nullptr)
            {
                int distance = abs(x - x0) + abs(y - y0);
                if(distance < min_distance)
                {
                    min_distance = distance;
                    fx = x;
                    fy = y;
                }
            }
    // Calculate the priority of direction.
    int dx = (fx != -1) ? (fx - x0) : 0;
    int dy = (fy != -1) ? (fy - y0) : 0;
    bool prioritize_horizontal
    = (abs(dx) > abs(dy) * 2); // Prefer horizontal moving.
    int pre_key = snake.getPre_key();
    // Set the direction priorities.
    // (First about food direction then random safety direction)
    int direction[4] = {UP, DOWN, LEFT, RIGHT};
    if(fx != -1)
    {
        if(prioritize_horizontal)
        {
            direction[0] = (dx > 0) ? RIGHT : LEFT;
            direction[1] = (dy > 0) ? DOWN : UP;
        }
        else
        {
            direction[0] = (dy > 0) ? DOWN : UP;
            direction[1] = (dx > 0) ? RIGHT : LEFT;
        }
    }
    // Check each direction by priority.
    for(int i = 0; i < 4; i++)
    {
        int this_key = direction[i];
        // Skip opposite direction.
        if(pre_key != -1) 
        {
            if((pre_key == UP && this_key == DOWN) ||
            (pre_key == DOWN && this_key == UP) ||
            (pre_key == LEFT && this_key == RIGHT) ||
            (pre_key == RIGHT && this_key == LEFT)) 
            continue;
        }
        // Check if the next move is safe.
        int nx = x0, ny = y0;
        switch(this_key)
        {
            case UP:    ny--; break;
            case DOWN:  ny++; break;
            case LEFT:  nx -= 2; break;
            case RIGHT: nx += 2; break;
        }
        if(nx < 0 || nx >= MAP_W ||
        ny < 0 || ny >= MAP_H) continue;
        if(!Map::checkOccupied(nx, ny) && 
        Snake::checkSnake(nx, ny) == nullptr) 
        {return this_key;}
    }
    // No safety directions.
    return UP;
}

void Game::transform(Snake* snake, Food* food)
{
	for(int i = 1; i < snake->getLen(); i++)
	{
		Snake::setSnake(snake->getX()[i], snake->getY()[i], nullptr);
		food -> create_as_snake_dies(snake->getX()[i], snake->getY()[i]);
	}
	Snake::setSnake(snake->get_x_tail(), snake->get_y_tail(), nullptr);
	food -> create_as_snake_dies(snake->get_x_tail(), snake->get_y_tail());
}