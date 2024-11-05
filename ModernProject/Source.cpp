#include<iostream>
#include"GameMap.h"
#include"Player.h"

int main()
{	
	GameMap map(20, 20);
	map.generateMap();
	map.displayMap();

	Player player("Hero", {2, 5}, 1);
	player.Display();
	return 0;
}