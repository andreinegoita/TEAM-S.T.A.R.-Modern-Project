﻿#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
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

#include "../PowerUps/PowerUps.h"
//#include"D:/ModernProject/PowerUps/PowerUps/PowerUps.h"
//#include "C:/Users/onetr/TeamStar/PowerUps/PowerUps/PowerUps.h"

void RunServer(GameMap &map, Player &player, http::Storage& storage)
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/map").methods("GET"_method)([&map]() {
		return crow::response(map.GetMapState());
		});

	CROW_ROUTE(app, "/map/empty/<int>/<int>").methods("POST"_method)
		([&map](int row, int col) {
		map.UpdateCell(row, col, 0U);
		return crow::response("Cell updated");
			});
	CROW_ROUTE(app, "/player_position").methods("GET"_method)
		([&player]() {
		return crow::response(player.GetPositionState());
			});
	CROW_ROUTE(app, "/player_position").methods("POST"_method)([&player](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON");
		}

		int x = body["x"].i();
		int y = body["y"].i();

		std::cout << "Received player position: x = " << x << ", y = " << y << std::endl;
		return crow::response(200);
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

	//CROW_ROUTE(app, "/shoot_bullet").methods("POST"_method)([](const crow::request& req) {
	//	auto json = crow::json::load(req.body);

	//	if (!json) {
	//		return crow::response(400, "Invalid JSON");
	//	}

	//	// Extract bullet data from JSON
	//	float x = json["x"].d();
	//	float y = json["y"].d();
	//	int direction = json["direction"].i();
	//	float speed = json["speed"].d();

	//	// Process the bullet
	//	handleIncomingBullet(x, y, direction, speed);

	//	return crow::response(200, "Bullet received and processed");
	//});



	// Bullet update endpoint
	/*CROW_ROUTE(app, "/get_bullets").methods("GET"_method)([&activeBullets] {
		crow::json::wvalue response;
		response["bullets"] = crow::json::wvalue::list();
		for (const auto& bullet : activeBullets) {
			crow::json::wvalue bulletJson;
			bulletJson["x"] = bullet.x;
			bulletJson["y"] = bullet.y;
			bulletJson["direction"] = bullet.direction;
			bulletJson["speed"] = bullet.speed;
			response["bullets"].push_back(bulletJson);
		}
		return crow::response(response);
		});*/

//	CROW_ROUTE(app, "/remove_bullet").methods("POST"_method)([&activeBullets](const crow::request& req) {
//		auto jsonReq = crow::json::load(req.body);
//		if (!jsonReq) {
//			return crow::response(400, "Invalid JSON");
//		}
//
//		float x = jsonReq["x"].d();
//		float y = jsonReq["y"].d();
//
//		// Find and remove the bullet that matches the position
//		auto it = std::remove_if(activeBullets.begin(), activeBullets.end(),
//			[x, y](const m_bulletData& bullet) {
//				return std::abs(bullet.x - x) < 10 && std::abs(bullet.y - y) < 10;  // 10-pixel threshold
//			});
//
//		if (it != activeBullets.end()) {
//			activeBullets.erase(it, activeBullets.end());
//			return crow::response(200, "Bullet removed");
//		}
//		return crow::response(404, "Bullet not found");
//		});
//
//
	CROW_ROUTE(app, "/buyPowerUp").methods("POST"_method)([&player](const crow::request& req) {
		std::cout << "Received POST request at /buyPowerUp\n";

		auto body = crow::json::load(req.body);
		if (!body) {
			std::cout << "Invalid JSON received\n";
			return crow::response(400, "Invalid JSON");
		}

		std::string powerUpTypeStr = body["powerUpType"].s();
		std::cout << "Power-up type received: " << powerUpTypeStr << "\n";

		if (powerUpTypeStr == "SpeedBoost") {
			player.BuyPowerUp(PowerUpType::SpeedBoost);
		}
		else if (powerUpTypeStr == "Shield") {
			player.BuyPowerUp(PowerUpType::Shield);
		}
		else if (powerUpTypeStr == "ExtraLife") {
			player.BuyPowerUp(PowerUpType::ExtraLife);
		}
		else {
			return crow::response(400, "Invalid power-up type");
		}

		return crow::response(player.GetPowerUpState());
		});
	CROW_ROUTE(app, "/powerUpQueue").methods("GET"_method)([&player]() {
		std::string powerUpState = player.GetPowerUpState();

		return crow::response(powerUpState);
		});
	CROW_ROUTE(app, "/applyPowerUp").methods("POST"_method)([&](const crow::request& req) {
		auto body = crow::json::load(req.body);

		if (!body) {
			return crow::response(400, "Invalid JSON");
		}

		PowerUpType type = static_cast<PowerUpType>(body["powerUpType"].i());

		player.ApplyPowerUpEffect(type);

		player.updatePowerUps();

		crow::json::wvalue response;


		response["bulletSpeed"] = player.GetBulletSpeed();


		response["lives"] = player.GetLives();
		response["hasShield"] = player.HasShield();

		return crow::response(200, response);
		});


	app.port(18080).multithreaded().run();
}

int main()
{
	//std::vector<m_bulletData> activeBullets;
	PowerUpType type = PowerUpType::Shield;
	int duration = 10;
	PowerUp* powerUp = createPowerUp(type, duration);
	std::cout << "PowerUp Type: " << static_cast<int>(powerUp->GetType()) << std::endl;
	std::cout << "Duration: " << powerUp->GetDuration() << std::endl;
	destroyPowerUp(powerUp);
	try {
		std::mt19937 mt(time(nullptr));
		int randValRows = 10 + mt() % 6;
		int randValCols = 10 + mt() % 20;
		GameMap map(randValRows, randValCols);

		map.generateMap();

		if (map.isValidMap()) {
			std::cout << map;
			Player player("Hero", { 0, 0 }, 3, DirectionType::Up);
			Player player2("Hero2", { 0, randValCols-1 }, 3, DirectionType::Up);
			Player player3("Hero3", { randValRows-1, 0 }, 3, DirectionType::Up);
			Player player4("Hero4", { randValRows-1,randValCols-1 }, 3, DirectionType::Up);
			system("cls");
			Weapon weapon(23, 54, 4.3, 2.4, DirectionType::Up);
			player.GetStartPosition();
			map.setCellType(player.GetStartPosition().first, player.GetStartPosition().second, CellType::Player);

			player2.GetStartPosition();
			map.setCellType(player2.GetStartPosition().first, player2.GetStartPosition().second, CellType::Player);

			/*player3.GetStartPosition();
			map.setCellType(player3.GetStartPosition().first, player3.GetStartPosition().second, CellType::Player);*/

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