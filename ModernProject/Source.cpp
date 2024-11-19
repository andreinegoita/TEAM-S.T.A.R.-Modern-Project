#include<iostream>
#include<random>
#include"GameMap.h"
#include"Player.h"
#include<chrono>
#include<thread>
#include<cstdlib>

int main()
{	
	std::mt19937 mt(time(nullptr));
	int randValRows = 10 + mt() % 10;
	int randValCols = 10 + mt() % 10;
	GameMap map(randValRows,randValCols);

	map.generateMap();
	std::cout << map;
	Player player("Hero", {0, 0}, 1,DirectionType::Up);
	player.Display();
	system("cls");
	Weapon weapon(23,54,4.3,2.4,DirectionType::Up);
	player.GetStartPosition();
	map.setCellType(player.GetStartPosition().first, player.GetStartPosition().second, CellType::Player);
	while (true)
	{
		if (_kbhit())
		{
			player.handleInput(map);
			system("cls");
			std::cout << map;
		}
		player.updateBullets(map);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	return 0;
}