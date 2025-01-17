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
			if (map.getCellType(row, col) ==CellType::EMPTY) { 
				availableSpawnPositions.insert({ row, col });
			}
		}
	}
}

void RunServer(GameMap &map, Player &player, http::Storage& storage, std::set<std::pair<int, int>> availableSpawnPositions)
{
	crow::SimpleApp app;
	app.loglevel(crow::LogLevel::Critical);
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

	std::map<std::string, std::tuple<int, int, int>> playerPositions;
	std::mutex playerPositionsMutex;

	CROW_ROUTE(app, "/player_position").methods("POST"_method)([&map, &playerPositions, &playerPositionsMutex](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON");
		}

		std::string playerName = body["name"].s();
		int x = body["x"].i();
		int y = body["y"].i();
		int direction = body["direction"].i(); 
		int lastX = body["lx"].i();
		int lastY = body["ly"].i();

		{
			std::lock_guard<std::mutex> lock(playerPositionsMutex);
			map.UpdateCell(lastY, lastX, 0U); 
			map.UpdateCell(y, x, 3U);         
			playerPositions[playerName] = std::make_tuple(x, y, direction); 
		}

		return crow::response(200);
		});
	CROW_ROUTE(app, "/get_all_players_positions").methods("GET"_method)([&playerPositions, &playerPositionsMutex]() {
		std::lock_guard<std::mutex> lock(playerPositionsMutex);

		crow::json::wvalue response;
		for (const auto& [playerName, position] : playerPositions) {
			int x = std::get<0>(position); 
			int y = std::get<1>(position);
			int direction = std::get<2>(position);

			response[playerName] = crow::json::wvalue{
				{"x", x},
				{"y", y},
				{"direction", direction} 
			};
		}
		return crow::response(200, response);
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
					}
					else {
						std::cout << "Bullet hit player at (" << x << ", " << y << "), player damaged!\n";
						player.DecreaseHealth(1);
						if (player.GetLives() <= 0) {
							std::cout << "Player at (" << x << ", " << y << ") has died.\n";
							map.UpdateCell(y, x, 0U); 
						}
					}
					map.UpdateCell(prevy, prevx, 0U); 
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

		return crow::response(200);
		});


	CROW_ROUTE(app, "/players").methods("GET"_method)
		([&storage]() {
		auto players = storage.get_all<http::PlayersDatabase>();
		std::ostringstream os;
		for (const auto& player : players) {
			os << "ID: " << player.id << ", Name: " << player.name << ", Points: " << player.points << "\n";
		}
		return crow::response("Process worked");
		return crow::response(os.str());
	});
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
		//if (playerPoints <= 0) {
		//	playerPoints = 1000;
		//}
		try {

			sqlite3* db;
			const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";
			

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

		std::string name = body["name"].s();
		if (name.empty()) {
			return crow::response(400, "Player name cannot be empty");
		}

		try {
			sqlite3* db;
			const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";

			if (sqlite3_open(db_name, &db) != SQLITE_OK) {
				return crow::response(500, "Failed to connect to database");
			}

			const char* select_sql = "SELECT name, points FROM players WHERE name = ? COLLATE NOCASE";
			sqlite3_stmt* stmt;

			if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) != SQLITE_OK) {
				sqlite3_close(db);
				return crow::response(500, "Failed to prepare SELECT statement");
			}

			sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
			std::cout << "Executing query: " << select_sql << " with name: " << name << std::endl;

			std::string playerName;
			int points = 0;

			if (sqlite3_step(stmt) == SQLITE_ROW) {
				playerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
				points = sqlite3_column_int(stmt, 1);
			}
			else {
				std::cout << "No player found with name: " << name << std::endl;
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return crow::response(404, "Player not found");
			}

			sqlite3_finalize(stmt);
			sqlite3_close(db);

			player.setName(playerName);
			player.setPoints(points);

			crow::json::wvalue response;
			response["status"] = "success";
			response["message"] = "Player found";
			response["name"] = playerName;
			response["points"] = points;
			return crow::response(200, response);

		}
		catch (const std::exception& e) {
			return crow::response(500, e.what());
		}
		});

	CROW_ROUTE(app, "/purchase").methods("POST"_method)([&storage](const crow::request& req) {
		auto body = crow::json::load(req.body);

		if (!body) {
			return crow::response(400, "Invalid JSON payload");
		}

		std::string playerName = body["name"].s();
		if (playerName.empty()) {
			return crow::response(400, "Player name cannot be empty");
		}

		int cost = body["cost"].i();
		if (cost <= 0) {
			return crow::response(400, "Invalid purchase cost");
		}

		try {
			sqlite3* db;
			const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";

			if (sqlite3_open(db_name, &db) != SQLITE_OK) {
				return crow::response(500, "Failed to connect to database");
			}

			const char* select_sql = "SELECT points FROM players WHERE name = ?;";
			sqlite3_stmt* select_stmt;

			if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr) != SQLITE_OK) {
				sqlite3_close(db);
				return crow::response(500, "Failed to prepare SELECT statement");
			}

			sqlite3_bind_text(select_stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);

			int playerPoints = -1;
			if (sqlite3_step(select_stmt) == SQLITE_ROW) {
				playerPoints = sqlite3_column_int(select_stmt, 0);
			}
			sqlite3_finalize(select_stmt);

			if (playerPoints == -1) {
				sqlite3_close(db);
				return crow::response(404, "Player not found");
			}

			if (playerPoints < cost) {
				sqlite3_close(db);
				return crow::response(400, "Insufficient points for purchase");
			}

			sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

			const char* update_sql = "UPDATE players SET points = points - ? WHERE name = ?;";
			sqlite3_stmt* update_stmt;

			if (sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, nullptr) != SQLITE_OK) {
				sqlite3_close(db);
				return crow::response(500, "Failed to prepare UPDATE statement");
			}

			sqlite3_bind_int(update_stmt, 1, cost);
			sqlite3_bind_text(update_stmt, 2, playerName.c_str(), -1, SQLITE_STATIC);

			if (sqlite3_step(update_stmt) != SQLITE_DONE) {
				std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
				sqlite3_finalize(update_stmt);
				sqlite3_close(db);
				return crow::response(500, "Failed to update player points");
			}

			sqlite3_finalize(update_stmt);
			sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
			sqlite3_close(db);

			crow::json::wvalue response;
			response["status"] = "success";
			response["name"] = playerName;
			response["remaining_points"] = playerPoints - cost;

			return crow::response(200, response);

		}
		catch (const std::exception& e) {
			return crow::response(500, e.what());
		}
		});

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
			auto players = storage.get_all<http::PlayersDatabase>();
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

		
		if (availableSpawnPositions.empty()) {
			return crow::response(400, "No available spawn points");
		}

		
		auto pos = *availableSpawnPositions.begin();
		availableSpawnPositions.erase(pos);
		map.UpdateCell(pos.first, pos.second, 3U);  

		crow::json::wvalue response;
		response["x"] = pos.first;
		response["y"] = pos.second;
		return crow::response(200, response);
		});


	app.port(18080).multithreaded().run();
}

int main()
{
	
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