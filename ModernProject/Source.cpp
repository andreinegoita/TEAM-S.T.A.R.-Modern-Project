#include<iostream>
#include"GameMap.h"
#include"Player.h"

int main()
{	
	GameMap map(20, 20);
	map.generateMap();
	std::cout << map;

	Player player("Hero", {2, 5}, 1);
	player.Display();
	Weapon weapon(23,54,4.3,2.4,1);
	weapon.Display();
	player.GetStartPosition();
	return 0;
}