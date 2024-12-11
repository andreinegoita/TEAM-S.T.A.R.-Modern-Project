#include "PlayersDatabase.h"
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
void http::populateStorage(Storage& storage, const std::string& playerName)
{
	Player newPlayer;
	newPlayer.name = playerName;
	newPlayer.points = 0;

	storage.insert(newPlayer);
}

void http::recordGame(Storage& storage, int playerId, int gameScore)
{
	Game newGame;
	newGame.playerId = playerId;
	newGame.score = gameScore;

	storage.insert(newGame);

	auto player = storage.get<Player>(playerId);
	player.points += gameScore;
	storage.update(player);
}
