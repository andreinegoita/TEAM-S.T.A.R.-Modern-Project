#include<iostream>
#include"GameMap.h"
#include"Player.h"

int main()
{	
	GameMap map(20, 20);
	map.generateMap();
	std::cout << map;

	Player player("Hero", {2, 5}, 1,DirectionType::Up);
	player.Display();
	Weapon weapon(23,54,4.3,2.4,DirectionType::Up);
	weapon.Display();
	player.GetStartPosition();
	player.Draw();
	return 0;
}