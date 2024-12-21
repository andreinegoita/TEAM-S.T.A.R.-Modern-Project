#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include<iostream>
#include<random>
#include"GameMap.h"
#include"Player.h"
#include "Bomb.h"
#include<crow.h>
#include<chrono>
#include<thread>
#include<cstdlib>
#include<stdexcept>
#include "PlayersDatabase.h"

#include <windows.h>
#include "PowerUpDll.h"
typedef PowerUp* (*CreatePowerUpFunc)(PowerUpType, int);
typedef void (*DestroyPowerUpFunc)(PowerUp*);

void RunServer(GameMap &map, Player &player, http::Storage& storage)
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/map").methods("GET"_method)([&map]() {
		return crow::response(map.GetMapState());
		});

	CROW_ROUTE(app, "/map/update/<int>/<int>/<int>").methods("POST"_method)
		([&map](int row, int col, int value) {
		map.UpdateCell(row, col, value);
		return crow::response("Cell updated");
			});
	CROW_ROUTE(app, "/player_position").methods("GET"_method)
		([&player]() {
		return crow::response(player.GetPositionState());
			});
	CROW_ROUTE(app, "/players").methods("GET"_method)
		([&storage]() {
		auto players = storage.get_all<http::Player>();
		std::ostringstream os;
		for (const auto& player : players) {
			os << "ID: " << player.id << ", Name: " << player.name << ", Points: " << player.points << "\n";
		}
		return crow::response("Process worked");
		return crow::response(os.str());
			});
	/*CROW_ROUTE(app, "/add_player").methods("POST"_method)
		([&storage](const crow::request& req) {
		auto x = crow::json::load(req.body);
		if (!x)
			return crow::response(400);
		http::Player new_player{ -1, x["name"].s(), x["points"].i() };
		storage.insert(new_player);
		return crow::response("Player added");
			});*/
	CROW_ROUTE(app, "/games").methods("GET"_method)
		([&storage]() {
		auto games = storage.get_all<http::Game>();
		std::ostringstream os;
		for (const auto& game : games) {
			os << "ID: " << game.id << ", Player ID: " << game.playerId << ", Score: " << game.score << "\n";
		}
		return crow::response("Process Game Worked");
		return crow::response(os.str());
			});
	/*CROW_ROUTE(app, "/add_game").methods("POST"_method) ([&storage]
		(const crow::request& req) {
			auto x = crow::json::load(req.body);
			if (!x)
				return crow::response(400);
			http::Game new_game{ -1, x["playerId"].i(), x["score"].i() };
			storage.insert(new_game);
			return crow::response("Game added");
		});*/

	CROW_ROUTE(app, "/register").methods("POST"_method)([&storage](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON");
		}

		std::string playerName = body["name"].s();
		if (playerName.empty()) {
			return crow::response(400, "Name cannot be empty");
		}

		try {
			http::populateStorage(storage, playerName);
			return crow::response(200, "Player registered successfully");
		}
		catch (const std::exception& e) {
			return crow::response(500, e.what());
		}
		});
	app.port(18080).multithreaded().run();
}

int main()
{	
		//HMODULE hDll = LoadLibrary(L"C:\\Users\\onetr\\TeamStar\\Dll\\Dll.dll");



		//if (hDll == NULL) {
		//	std::cerr << "Could not load the dynamic library!" << std::endl;
		//	return -1;
		//}

		//// Obține adresele funcțiilor
		//CreatePowerUpFunc createPowerUp = (CreatePowerUpFunc)GetProcAddress(hDll, "createPowerUp");
		//DestroyPowerUpFunc destroyPowerUp = (DestroyPowerUpFunc)GetProcAddress(hDll, "destroyPowerUp");

		//if (createPowerUp == NULL || destroyPowerUp == NULL) {
		//	std::cerr << "Could not find the function in the DLL!" << std::endl;
		//	FreeLibrary(hDll);
		//	return -1;
		//}

		//// Creează și folosește PowerUp-ul
		//PowerUp* powerUp = createPowerUp(PowerUpType::SpeedBoost, 10);
		//std::cout << "PowerUp Type: " << static_cast<int>(powerUp->getType()) << std::endl;
		//std::cout << "Duration: " << powerUp->getDuration() << std::endl;

		//// Distruge PowerUp-ul
		//destroyPowerUp(powerUp);

		//// Eliberează DLL-ul
		//FreeLibrary(hDll);


	try {
		std::mt19937 mt(time(nullptr));
		int randValRows = 10 + mt() % 10;
		int randValCols = 10 + mt() % 10;
		GameMap map(randValRows, randValCols);

		map.generateMap();

		if (map.isValidMap()) {
			std::cout << map;
			Player player("Hero", { 0, 6 }, 1, DirectionType::Up);
			Player player2("Hero2", { 0, 4 }, 1, DirectionType::Up);
			Player player3("Hero3", { 4, 0 }, 1, DirectionType::Up);
			Player player4("Hero4", { 5, 0 }, 1, DirectionType::Up);
			system("cls");
			Weapon weapon(23, 54, 4.3, 2.4, DirectionType::Up);
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

			auto storage = http::createStorage("game.db");
			storage.sync_schema(true);
			RunServer(map,player, storage);

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
		}
		else
			throw std::runtime_error("Map is not valid");
	}
	catch (const std::exception& e){
		std::cerr << "An error occured: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}