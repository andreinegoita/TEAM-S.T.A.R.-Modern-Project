#include "Player.h"
#include<iostream>

Player::Player(std::string_view name, std::pair<uint16_t, uint16_t> position, double velocity) :m_name{ name },
GameObject{ position, velocity }, m_health{ 3 }, m_points(0), m_score{0}
{
}
