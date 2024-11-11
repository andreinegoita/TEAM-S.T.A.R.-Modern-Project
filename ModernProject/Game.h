#pragma once
#include"GameMap.h"
#include"Player.h"
class Game
{
	Game(const GameMap& map, std::vector<Player*>players);


private:
	GameMap m_map;
	std::vector<Player*>m_players;

};

