#pragma once
#include<iostream>
#include<vector>
#include"Player.h"
#include"GameMap.h"
class Game
{
public:
	Game(const GameMap& map, std::vector<Player*>players);

private:
	GameMap m_map;
	std::vector<Player*>m_players;

};

