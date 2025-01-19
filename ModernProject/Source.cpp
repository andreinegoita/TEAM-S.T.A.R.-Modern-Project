import Bomb;
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include<iostream>
#include<random>
#include"GameMap.h"
#include"Player.h"
#include<crow.h>
#include<chrono>
#include<thread>
#include<cstdlib>
#include<stdexcept>
#include "PlayersDatabase.h"
#include <windows.h>
#include"Server.h"
#include<mutex>
#include "../PowerUps/PowerUps/PowerUps.h"

void initializeSpawnPositions(GameMap& map, std::set<std::pair<int, int>> availableSpawnPositions) {
	availableSpawnPositions.clear();
	for (int row = 0; row < map.getRows(); ++row) {
		for (int col = 0; col < map.getCols(); ++col) {
			if (map.getCellType(row, col) ==CellType::EMPTY) { 
				availableSpawnPositions.insert({ row, col });
			}
		}
	}
}

int main()
{
	
	try {
		std::mt19937 mt(time(nullptr));
		int randValRows = 10 + mt() % 6;
		int randValCols = 10 + mt() % 20;
		GameMap map(randValRows, randValCols);

		map.generateMap();
		
		if (map.isValidMap()) {
			std::cout << map;
			Player player("", { (randValRows / 2),0 }, 3, DirectionType::Up);
			map.setCellType(0, 0, CellType::EMPTY);
			map.setCellType((randValRows / 2), 0, CellType::UNBREAKABLE_WALL);

			map.setCellType(0, (randValCols / 2) + 1, CellType::EMPTY);
			map.setCellType(0, (randValCols / 2), CellType::EMPTY);
			map.setCellType(0, (randValCols / 2)-1, CellType::EMPTY);
			system("cls");
			Weapon weapon(23, 54, 4.3, 2.4, DirectionType::Up);
			player.GetStartPosition();
			map.setCellType(player.GetStartPosition().first, player.GetStartPosition().second, CellType::Player);


			for (int i = 0; i < 3; i++)
			{
				Bomb bomb;
				bomb.SetCoordinates(map);
			}

			auto storage = http::createStorage<http::PlayersDatabase<int, std::string, int>, http::Game<int, int, int>>("game.db");
			storage.sync_schema(true);  
			Server m_server;
			m_server.RunServer(map, player, storage);
		}
			
	}
	catch (const std::exception& e){
		std::cerr << "An error occured: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}