#include<iostream>
#include"GameMap.h"
#include"Player.h"

int main()
{	
	GameMap map(20, 20);
	map.generateMap();
	map.displayMap();

	Player player("Hero", {2, 5}, 1);
	std::cout << "Player speed :"<< player.GetSpeed()<<std::endl;
	std::cout<<"Position on matrix (X) :"<<player.GetX()<<std::endl;
	std::cout<<"Position on matrix (Y) :"<<player.GetY()<<std::endl;
	player.Display();
	return 0;
}