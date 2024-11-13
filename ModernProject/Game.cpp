#include "Game.h"

Game::Game(const GameMap& map, std::vector<Player*> players) : m_map{ map }, m_players{ players } {}
