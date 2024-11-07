#include "Bomb.h"
#include<cmath>

Bomb::Bomb(uint8_t x, uint8_t y, uint16_t radius, bool isActivated):
	GameObject({ x,y },0), m_radius(radius), m_isActivated(isActivated)
{}


uint16_t Bomb::getRadius() const
{
	return m_radius;
}

bool Bomb::getIsActivated() const
{
	return m_isActivated;
}


void Bomb::setRadius(uint16_t radius)
{
	this->m_radius = radius;
}

void Bomb::setIsArmed(bool isActivated)
{
	this->m_isActivated = isActivated;
}

bool Bomb::isWithinRange(int playerX, int playerY) const
{
	int dx = static_cast<int>(m_position.first) - playerX;
	int dy = static_cast<int>(m_position.second) - playerY;
	return (dx * dx + dy * dy) <= (m_radius * m_radius);
}
