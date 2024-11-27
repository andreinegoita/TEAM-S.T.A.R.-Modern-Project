#include<iostream>
#include<random>
#include"GameMap.h"
#include"Player.h"
#include "Bomb.h"
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
	Player player2("Hero2", {0, 4}, 1,DirectionType::Up);
	Player player3("Hero3", {4, 0}, 1,DirectionType::Up);
	Player player4("Hero4", {5, 0}, 1,DirectionType::Up);
	system("cls");
	Weapon weapon(23,54,4.3,2.4,DirectionType::Up);
	player.GetStartPosition();
	map.setCellType(player.GetStartPosition().first, player.GetStartPosition().second, CellType::Player);

	player2.GetStartPosition();
	map.setCellType(player2.GetStartPosition().first, player2.GetStartPosition().second, CellType::Player);

	player3.GetStartPosition();
	map.setCellType(player3.GetStartPosition().first, player3.GetStartPosition().second, CellType::Player);

	player4.GetStartPosition();
	map.setCellType(player4.GetStartPosition().first, player4.GetStartPosition().second, CellType::Player);


	for (int i = 0; i < 3; i++)
	{
		Bomb bomb;
		bomb.SetCoordinates(map);
	}

	while (true)
	{
		if (_kbhit())
		{
			player.HandleInput(map);
			system("cls");
			std::cout << map;
		}
		player.UpdateBullets(map);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	return 0;
}