#include "PlayersDatabase.h"
#include <cpr/cpr.h>
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
void http::populateStorage(Storage& storage, const std::string& playerName)
{
	PlayersDatabase<int,std::string,int> newPlayer;
	newPlayer.name = playerName;
	newPlayer.points = 3000;

	storage.insert(newPlayer);
}

void http::recordGame(Storage& storage, int playerId, int gameScore)
{
	Game<int,int,int> newGame;
	newGame.playerId = playerId;
	newGame.score = gameScore;

	storage.insert(newGame);

	auto player = storage.get<http::PlayersDatabase<int, std::string, int>>(playerId);
	player.points += gameScore;
	storage.update(player);
}

void http::executePowerUpTransaction(int points,std::string playerName) {
    std::string url = "http://localhost:18080/purchase";

    // Convert points to a string
    std::string pointsStr = std::to_string(points);
    // Create the payload with the player's name and points
    cpr::Payload payload = cpr::Payload{
        {"name", playerName},
        {"cost", pointsStr}    
    };

    // Send the POST request
    cpr::Response response = cpr::Post(cpr::Url{ url }, payload);
}