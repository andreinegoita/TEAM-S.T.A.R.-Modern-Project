#include<iostream>
#include<random>
#include"GameMap.h"
#include"Player.h"

int main()
{	
	std::mt19937 mt(time(nullptr));
	int randValRows = 10 + mt() % 10;
	int randValCols = 10 + mt() % 10;
	GameMap map(randValRows,randValCols);

	map.generateMap();
	std::cout << map;

	Player player("Hero", {2, 5}, 1,DirectionType::Up);
	player.Display();
	Weapon weapon(23,54,4.3,2.4,DirectionType::Up);
	weapon.Display();
	player.GetStartPosition();
	player.Draw();
	while (true)
	{
		player.handleInput(map);
	}
	return 0;
}