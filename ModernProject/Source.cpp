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
#include<mutex>


#include "../PowerUps/PowerUps.h"
//#include"D:/ModernProject/PowerUps/PowerUps/PowerUps.h"
//#include "C:/Users/onetr/TeamStar/PowerUps/PowerUps/PowerUps.h"

void initializeSpawnPositions(GameMap& map, std::set<std::pair<int, int>> availableSpawnPositions) {
	availableSpawnPositions.clear();
	for (int row = 0; row < map.getRows(); ++row) {
		for (int col = 0; col < map.getCols(); ++col) {
			if (map.getCellType(row, col) ==CellType::EMPTY) { // Check for empty cell
				availableSpawnPositions.insert({ row, col });
			}
		}
	}
}

void RunServer(GameMap &map, Player &player, http::Storage& storage, std::set<std::pair<int, int>> availableSpawnPositions)
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
	CROW_ROUTE(app, "/player_position").methods("POST"_method)([&map](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON");
		}



		int x = body["x"].i();
		int y = body["y"].i();
		int lastx = body["lx"].i();
		int lasty = body["ly"].i();

		map.UpdateCell(lasty, lastx, 0U);
		map.UpdateCell(y, x, 3U);

		std::cout << "Received player position: x = " << x << ", y = " << y << std::endl;
		return crow::response(200);
		});
	CROW_ROUTE(app, "/bullets_position").methods("POST"_method)([&map, &player](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON");
		}

		for (const auto& bullet : body) {
			int x = bullet["x"].i();
			int y = bullet["y"].i();
			int prevx = bullet["prev_x"].i();
			int prevy = bullet["prev_y"].i();
			int direction = bullet["direction"].i();

			try {
				CellType currentCellType = map.getCellType(y, x);

				if (currentCellType == CellType::UNBREAKABLE_WALL) {
					std::cout << "Bullet hit an unbreakable wall at (" << x << ", " << y << ")\n";
					map.UpdateCell(prevy, prevx, 0U); 
					continue; 
				}


				if (currentCellType == CellType::Player) {
					if (player.HasShield()) {
						std::cout << "Bullet hit player with shield at (" << x << ", " << y << "), shield absorbed the bullet\n";
						map.UpdateCell(prevy, prevx, 0U);  
					}
					else {
						std::cout << "Bullet hit player at (" << x << ", " << y << "), player damaged!\n";
						map.UpdateCell(y, x, 0U);  
						map.UpdateCell(prevy, prevx, 0U);  
					}
					continue; 
				}


				if (currentCellType == CellType::Bullet) {
					std::cout << "Bullet collided with another bullet at (" << x << ", " << y << ")\n";
					map.UpdateCell(y, x, 0U);
					map.UpdateCell(prevy, prevx, 0U);  
					continue; 
				}

				if (currentCellType == CellType::EMPTY) {
					map.UpdateCell(y, x, 4U);
					map.UpdateCell(prevy, prevx, 0U);
				}

			}
			catch (const std::out_of_range& e) {
				std::cerr << "Out of bounds: " << e.what() << "\n";
			}

			std::cout << "Bullet at (" << x << ", " << y << ", " << prevx << ", " << prevy << "), direction: " << direction << std::endl;
		}

		return crow::response(200);  // Răspuns de succes
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

	//CROW_ROUTE(app, "/register").methods("POST"_method)([&storage](const crow::request& req) {
	//	auto body = crow::json::load(req.body);

	//	// Verificare validitate JSON
	//	if (!body) {
	//		return crow::response(400, "Invalid JSON payload");
	//	}

	//	// Extrage datele din JSON
	//	std::string playerName = body["name"].s();
	//	if (playerName.empty()) {
	//		return crow::response(400, "Player name cannot be empty");
	//	}

	//	int playerPoints = body["points"].i(); // Opțional, poate avea un număr implicit
	//	if (playerPoints <= 0) {
	//		playerPoints = 300; // Valoare implicită
	//	}

	//	try {
	//		
	//		http::populateStorage(storage, playerName);

	//		// Răspuns JSON către client
	//		crow::json::wvalue response;
	//		response["status"] = "success";
	//		response["name"] = playerName;
	//		response["points"] = playerPoints;

	//		return crow::response(200, response);
	//	}
	//	catch (const std::exception& e) {
	//		return crow::response(500, e.what());
	//	}
	//	});


	CROW_ROUTE(app, "/register").methods("POST"_method)([&storage](const crow::request& req) {
		auto body = crow::json::load(req.body);


		if (!body) {
			return crow::response(400, "Invalid JSON payload");
		}

		
		std::string playerName = body["name"].s();
		if (playerName.empty()) {
			return crow::response(400, "Player name cannot be empty");
		}

		int playerPoints = body["points"].i();
		if (playerPoints <= 0) {
			playerPoints = 300;
		}

		try {

			sqlite3* db;
			const char* db_name = "D:/TEAM-S.T.A.R.-Modern-Project/ModernProject/game.db";
			

			if (sqlite3_open(db_name, &db) != SQLITE_OK) {
				return crow::response(500, "Failed to connect to database");
			}

			const char* select_sql = "SELECT COUNT(*) FROM players WHERE name = ?";
			sqlite3_stmt* select_stmt;

			if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr) != SQLITE_OK) {
				sqlite3_close(db);
				return crow::response(500, "Failed to prepare SELECT statement");
			}

			sqlite3_bind_text(select_stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);
			int count = 0;

			if (sqlite3_step(select_stmt) == SQLITE_ROW) {
				count = sqlite3_column_int(select_stmt, 0);
			}

			sqlite3_finalize(select_stmt);

			if (count > 0) {
				sqlite3_close(db);
				return crow::response(409, "Player name already exists");
			}

			const char* insert_sql = "INSERT INTO players (name, points) VALUES (?, ?)";
			sqlite3_stmt* insert_stmt;

			if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr) != SQLITE_OK) {
				sqlite3_close(db);
				return crow::response(500, "Failed to prepare INSERT statement");
			}

			sqlite3_bind_text(insert_stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_int(insert_stmt, 2, playerPoints);

			if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
				sqlite3_finalize(insert_stmt);
				sqlite3_close(db);
				return crow::response(500, "Failed to insert player into database");
			}

			sqlite3_finalize(insert_stmt);
			sqlite3_close(db);

			crow::json::wvalue response;
			response["status"] = "success";
			response["name"] = playerName;
			response["points"] = playerPoints;

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, e.what());
		}
		});
	CROW_ROUTE(app, "/login").methods("GET"_method)([&player](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON body");
		}

		std::string id = body["id"].s();
		std::string name = body["name"].s();
		int points = body["points"].i();
		player.setName(name);
		player.setPoints(points);


		crow::json::wvalue response;
		response["status"] = "success";
		response["message"] = "Player found";
		response["points"] = points;
		return crow::response(200, response);

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
		else if (powerUpTypeStr == "FireRate")
		{
			player.BuyPowerUp(PowerUpType::FireRate);
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
		response["fireRate"] = player.GetFireRate();

		return crow::response(200, response);
		});
	CROW_ROUTE(app, "/get_players").methods("GET"_method)([&storage]() {
		try {
			auto players = storage.get_all<http::Player>();
			crow::json::wvalue response;


			response["players"] = crow::json::wvalue::list();


			size_t index = 0;
			for (const auto& player : players) {
				crow::json::wvalue playerJson;
				playerJson["id"] = player.id;
				playerJson["name"] = player.name;
				playerJson["points"] = player.points;

				response["players"][index++] = std::move(playerJson);
			}

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, e.what());
		}
		});

	CROW_ROUTE(app, "/getPoints").methods("GET"_method)([&player]() {

		crow::json::wvalue points;
		points["playerPoints"] =player.GetPoints();

		return crow::response(points);

		});
	std::vector<std::string> lobbyPlayers;
	std::mutex lobbyMutex;

	CROW_ROUTE(app, "/join_lobby").methods("POST"_method)([&lobbyMutex,&lobbyPlayers](const crow::request& req) {
		auto body = crow::json::load(req.body);
		std::string playerName = body["name"].s();

		std::lock_guard<std::mutex> lock(lobbyMutex);
		if (std::find(lobbyPlayers.begin(), lobbyPlayers.end(), playerName) == lobbyPlayers.end()) {
			lobbyPlayers.push_back(playerName);
		}

		crow::json::wvalue response;
		response["players_in_lobby"] = lobbyPlayers.size();
		return crow::response(200, response);
		});

	CROW_ROUTE(app, "/get_lobby_players").methods("GET"_method)([&lobbyMutex, &lobbyPlayers]() {
		crow::json::wvalue response;
		std::lock_guard<std::mutex> lock(lobbyMutex);

		
		crow::json::wvalue::list playersList;
		for (const auto& player : lobbyPlayers) {
			playersList.emplace_back(player);
		}
		response["players"] = std::move(playersList);  

		return crow::response(200, response);
		});

	CROW_ROUTE(app, "/map/player/<int>/<int>").methods("POST"_method)
		([&map](int row, int col) {
		map.UpdateCell(row, col, 3U);
		return crow::response("Cell updated");
			});
	std::mutex positionMutex;
	CROW_ROUTE(app, "/request_spawn").methods("POST"_method)([&map,&positionMutex,&availableSpawnPositions](const crow::request& req) {
		auto body = crow::json::load(req.body);
		std::string playerName = body["name"].s();

		if (!body) {
			return crow::response(400, "Invalid JSON");
		}

		std::lock_guard<std::mutex> lock(positionMutex);

		// If there are no available positions, return error
		if (availableSpawnPositions.empty()) {
			return crow::response(400, "No available spawn points");
		}

		// Select a free position and remove it from the set
		auto pos = *availableSpawnPositions.begin();
		availableSpawnPositions.erase(pos);
		map.UpdateCell(pos.first, pos.second, 3U);  // Mark as occupied

		crow::json::wvalue response;
		response["x"] = pos.first;
		response["y"] = pos.second;
		return crow::response(200, response);
		});


	app.port(18080).multithreaded().run();
}

int main()
{
	//std::vector<m_bulletData> activeBullets;
	try {
		std::mt19937 mt(time(nullptr));
		int randValRows = 10 + mt() % 6;
		int randValCols = 10 + mt() % 20;
		GameMap map(randValRows, randValCols);

		map.generateMap();
		std::set<std::pair<int, int>> availableSpawnPosition = { {0,0},{0,randValCols - 1},{randValRows - 1,0 },{randValRows - 1,randValCols - 1} };
		if (map.isValidMap()) {
			std::cout << map;
			Player player("Hero", { (randValRows/2)-1,0}, 3, DirectionType::Up);
			map.setCellType(0, 0, CellType::EMPTY);
			map.setCellType((randValRows/2)-1, 0, CellType::Player);
			//Player player2("Hero2", { 0, randValCols-1 }, 3, DirectionType::Up);
			//Player player3("Hero3", { randValRows-1, 0 }, 3, DirectionType::Up);
			//Player player4("Hero4", { randValRows-1,randValCols-1 }, 3, DirectionType::Up);
			system("cls");
			Weapon weapon(23, 54, 4.3, 2.4, DirectionType::Up);
			player.GetStartPosition();
			map.setCellType(player.GetStartPosition().first, player.GetStartPosition().second, CellType::Player);

			//player2.GetStartPosition();
			//map.setCellType(player2.GetStartPosition().first, player2.GetStartPosition().second, CellType::Player);

			/*player3.GetStartPosition();
			map.setCellType(player3.GetStartPosition().first, player3.GetStartPosition().second, CellType::Player);*/

			//player4.GetStartPosition();
			//map.setCellType(player4.GetStartPosition().first, player4.GetStartPosition().second, CellType::Player);


			for (int i = 0; i < 3; i++)
			{
				Bomb bomb;
				bomb.SetCoordinates(map);
			}

			auto storage = http::createStorage("game.db");
			storage.sync_schema(true);
			RunServer(map,player, storage, availableSpawnPosition);

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