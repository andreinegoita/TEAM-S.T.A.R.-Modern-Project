#include "Player.h"
#include<iostream>

Player::Player( std::string_view  name, std::pair<uint16_t, uint16_t> position, float velocity) :m_name{ name },
GameObject{ position, velocity }, m_health{ 3 }, m_points(0), m_score{0}
{
}

void Player::Shoot(uint8_t direction)
{
	auto newBullet = m_weapon.fire(m_position.first, m_position.second,m_direction);
	if (newBullet)
	{
		m_bullets.push_back(std::move(newBullet));
	}
}

void Player::Display()
{
	std::cout << "Player " << m_name << "located at {" << m_position.first << ' ' << m_position.second << "} with velocity "
		<< m_velocity << "health " << m_health << "points " << m_points << "score " << m_score << std::endl;
}
