#include "Player.h"
#include<iostream>
Player::Player(std::string_view name,std::pair<uint16_t, uint16_t>location,uint16_t health,double movespeed):
	m_name{ name }, m_location{ location }, m_health{ health }, m_movespeed{ movespeed }, m_score{ 0 }, m_points{0} {}
