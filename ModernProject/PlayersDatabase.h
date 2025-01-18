#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include<crow.h>
#include<string>
#include<sqlite_orm/sqlite_orm.h>
#include<type_traits>

namespace sql = sqlite_orm;

namespace http
{
	template <typename IdType, typename NameType, typename PointsType>
	struct PlayersDatabase {
		IdType id;
		NameType name;
		PointsType points;
	};

	template <typename IdType, typename PlayerIdType, typename ScoreType>
	struct Game {
		IdType id;
		PlayerIdType playerId;
		ScoreType score;
	};

	template <typename PlayersDatabase, typename Game>
	inline auto createStorage(const std::string& filename) {
		return sql::make_storage(
			filename,
			sql::make_table(
				"Players",
				sql::make_column("id", &PlayersDatabase::id, sql::primary_key().autoincrement()),
				sql::make_column("name", &PlayersDatabase::name),
				sql::make_column("points", &PlayersDatabase::points)
			),
			sql::make_table(
				"Games",
				sql::make_column("id", &Game::id, sql::primary_key().autoincrement()),
				sql::make_column("playerId", &Game::playerId),
				sql::make_column("score", &Game::score)
			)
		);
	}

	using Storage = decltype(createStorage<PlayersDatabase<int, std::string, int>, Game<int, int, int>>(""));


	void populateStorage(Storage& storage, const std::string& playerName);

	void recordGame(Storage& storage, int playerId, int gameScore);
	void executePowerUpTransaction(int points, std::string playerName);
}

