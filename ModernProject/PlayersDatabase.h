#pragma once
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

	inline auto createStorage(const std::string& filename) {
		return sql::make_storage(
			filename,
			sql::make_table(
				"Products",
				sql::make_column("id", &Player::id, sql::primary_key().autoincrement()),
				sql::make_column("name", &Player::name),
				sql::make_column("points", &Player::points)
			)
		);
	}

	using Storage = decltype(createStorage(""));

	void populateStorage(Storage& storage, const std::string& playerName);

}

