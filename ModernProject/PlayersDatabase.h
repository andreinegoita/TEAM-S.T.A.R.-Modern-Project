#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include<string>
#include<crow.h>
#include<sqlite_orm/sqlite_orm.h>

namespace sql = sqlite_orm;

namespace http
{
	struct Player {
		int id;
		std::string name;
		int points;
	};

	struct Game {
		int id;
		int playerId;
		int score;
	};

	inline auto createStorage(const std::string& filename) {
		return sql::make_storage(
			filename,
			sql::make_table(
				"Players",
				sql::make_column("id", &Player::id, sql::primary_key().autoincrement()),
				sql::make_column("name", &Player::name),
				sql::make_column("points", &Player::points)
			),
			sql::make_table(
				"Games",
				sql::make_column("id", &Game::id, sql::primary_key().autoincrement()),
				sql::make_column("playerId", &Game::playerId),
				sql::make_column("score", &Game::score)
			)
		);
	}

	using Storage = decltype(createStorage(""));

	void populateStorage(Storage& storage, const std::string& playerName);

	void recordGame(Storage& storage, int playerId, int gameScore);
}

