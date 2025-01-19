#include "Server.h"

void Server::RunServer(GameMap& map, Player& player, http::Storage& storage)
{
	crow::SimpleApp app;
	app.loglevel(crow::LogLevel::Critical);
	CROW_ROUTE(app, "/map").methods("GET"_method)([&map]() {
		return crow::response(map.GetMapState());
		});

	CROW_ROUTE(app, "/player_position").methods("GET"_method)
		([&player]() {
		return crow::response(player.GetPositionState());
			});

	std::map<std::string, std::tuple<int, int, int, int>> playerPositions;
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
			playerPositions[playerName] = std::make_tuple(x, y, direction,3);
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
		auto players = storage.get_all<http::PlayersDatabase<int, std::string, int>>();
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
		try {

			sqlite3* db;
			//const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";
			//const char* db_name = "C:\\Users\\Sebi\\Desktop\\ModernProject\\ModernProject/game.db";
			const char* db_name = "D:\\ModernProject\\ModernProject/game.db";

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

	std::vector<std::string> playersNames;
	std::unordered_map<std::string, int> playerLives;
	CROW_ROUTE(app, "/login").methods("POST"_method)([&player, &playersNames,&playerLives](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON body");
		}

		std::string name = body["name"].s();

		if (playerLives.find(name) == playerLives.end()) {
			playerLives[name] = 3; 
			std::cout << "Player " << name << " logged in with 3 lives.\n";
		}
		if (name.empty()) {
			return crow::response(400, "Player name cannot be empty");
		}

		try {
			sqlite3* db;
			//const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";
			//const char* db_name = "C:\\Users\\Sebi\\Desktop\\ModernProject\\ModernProject/game.db";
			const char* db_name = "D:\\ModernProject\\ModernProject/game.db";

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

			std::string playerName;
			int points = 0;

			if (std::find(playersNames.begin(), playersNames.end(), name) != playersNames.end()) {
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return crow::response(409, "Player already logged in");
			}

			if (sqlite3_step(stmt) == SQLITE_ROW) {
				playerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
				points = sqlite3_column_int(stmt, 1);
			}
			else {
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return crow::response(404, "Player not found");
			}

			sqlite3_finalize(stmt);
			sqlite3_close(db);

			player.setName(playerName);
			player.setPoints(points);
			playersNames.push_back(playerName);

			crow::json::wvalue response;
			response["status"] = "success";
			response["message"] = "Player logged in successfully";
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
			//const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";
			//const char* db_name = "C:\\Users\\Sebi\\Desktop\\ModernProject\\ModernProject/game.db";
			const char* db_name = "D:\\ModernProject\\ModernProject/game.db";

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
			auto players = storage.get_all<http::PlayersDatabase<int, std::string, int>>();
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
		points["playerPoints"] = player.GetPoints();

		return crow::response(points);

		});
	std::vector<std::string> lobbyPlayers;
	std::mutex lobbyMutex;

	CROW_ROUTE(app, "/join_lobby").methods("POST"_method)([&lobbyMutex, &lobbyPlayers](const crow::request& req) {
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
	
	CROW_ROUTE(app, "/map/empty/<int>/<int>").methods("POST"_method)
		([&map](int row, int col) {
		std::cout << "Received request to update cell (" << row << ", " << col << ") to Empty.\n";
		map.UpdateCell(row, col, 0U);
		return crow::response("Cell updated");
			});

	CROW_ROUTE(app, "/update_player_lives")
		.methods("POST"_method)
		([&playerLives](const crow::request& req) {
		auto jsonData = crow::json::load(req.body);
		if (!jsonData) {
			return crow::response(400, "Invalid JSON");
		}

		std::string playerName = jsonData["name"].s();
		int lives = jsonData["lives"].i();

	
		if (playerLives.find(playerName) != playerLives.end()) {
			playerLives[playerName] = lives;

			if (playerLives[playerName] <= 0) {
				playerLives.erase(playerName);
				std::cout << "Player " << playerName << " has been removed from the game.\n";
			}

			return crow::response(200, "Player lives updated");
		}

		return crow::response(404, "Player not found");
			});

	CROW_ROUTE(app, "/get_player_lives")
		.methods("GET"_method)
		([&playerLives]() {
		crow::json::wvalue jsonResponse;
		for (const auto& [name, lives] : playerLives) {
			jsonResponse[name] = lives;
		}
		return crow::response(jsonResponse);
			});

	CROW_ROUTE(app, "/add_player_points").methods("POST"_method)([](const crow::request& req) {
		auto body = crow::json::load(req.body);
		if (!body) {
			return crow::response(400, "Invalid JSON payload");
		}

		std::string playerName = body["name"].s();
		int pointsToAdd = body["points"].i();

		if (playerName.empty()) {
			return crow::response(400, "Player name cannot be empty");
		}

		try {
			sqlite3* db;
			//const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";
			//const char* db_name = "C:\\Users\\Sebi\\Desktop\\ModernProject\\ModernProject/game.db";
			const char* db_name = "D:\\ModernProject\\ModernProject/game.db";

			if (sqlite3_open(db_name, &db) != SQLITE_OK) {
				return crow::response(500, "Failed to connect to database");
			}

			const char* update_sql = "UPDATE players SET points = points + ? WHERE name = ?;";
			sqlite3_stmt* stmt;

			if (sqlite3_prepare_v2(db, update_sql, -1, &stmt, nullptr) != SQLITE_OK) {
				sqlite3_close(db);
				return crow::response(500, "Failed to prepare UPDATE statement");
			}

			sqlite3_bind_int(stmt, 1, pointsToAdd);
			sqlite3_bind_text(stmt, 2, playerName.c_str(), -1, SQLITE_STATIC);

			if (sqlite3_step(stmt) != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return crow::response(500, "Failed to update player points");
			}

			sqlite3_finalize(stmt);
			sqlite3_close(db);

			crow::json::wvalue response;
			response["status"] = "success";
			response["message"] = "Points added successfully";
			response["name"] = playerName;
			response["added_points"] = pointsToAdd;

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, e.what());
		}
		});

	CROW_ROUTE(app, "/get_player_points/<string>").methods("GET"_method)([](const std::string& playerName) {
    if (playerName.empty()) {
        return crow::response(400, "Player name cannot be empty");
    }

    try {
        sqlite3* db;
		//const char* db_name = "C:\\Users\\onetr\\TeamStar\\ModernProject/game.db";
		//const char* db_name = "C:\\Users\\Sebi\\Desktop\\ModernProject\\ModernProject/game.db";
		const char* db_name = "D:\\ModernProject\\ModernProject/game.db";

        if (sqlite3_open(db_name, &db) != SQLITE_OK) {
            return crow::response(500, "Failed to connect to database");
        }

        const char* select_sql = "SELECT points FROM players WHERE name = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare SELECT statement");
        }

        sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);

        int points = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            points = sqlite3_column_int(stmt, 0);
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(404, "Player not found");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        crow::json::wvalue response;
        response["name"] = playerName;
        response["points"] = points;

        return crow::response(200, response);
    } catch (const std::exception& e) {
        return crow::response(500, e.what());
    }
});

	app.port(18080).multithreaded().run();
}