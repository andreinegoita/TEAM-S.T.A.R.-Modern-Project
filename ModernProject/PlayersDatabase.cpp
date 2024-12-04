#include "PlayersDatabase.h"

void http::populateStorage(Storage& storage, const std::string& playerName)
{
	Player newPlayer;
	newPlayer.name = playerName;
	newPlayer.points = 0;

	storage.insert(newPlayer);
}
