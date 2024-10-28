#include "Player.h"
#include<iostream>
Player::Player(const std::string_view& name, const std::pair<uint16_t, uint16_t> location):
	m_name{ name }, m_location{ location } {
}
