#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include"GameMap.h"
#include"Player.h"
#include "PlayersDatabase.h"
class Server
{
public:
	void RunServer(GameMap& map, Player& player, http::Storage& storage);
};

