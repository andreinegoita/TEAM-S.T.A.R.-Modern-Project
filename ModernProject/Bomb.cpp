#include "Bomb.h"
#include<cmath>

Bomb::Bomb(uint8_t x, uint8_t y, uint16_t radius, bool isActivated):
	m_x(x),m_y(y),m_radius(radius),m_isActivated(isActivated)
{}

uint8_t Bomb::getX() const
{
    return m_x;
}

uint8_t Bomb::getY() const
{
	return m_y;
}

uint16_t Bomb::getRadius() const
{
	return m_radius;
}

bool Bomb::getIsActivated() const
{
	return m_isActivated;
}

void Bomb::setX(uint8_t x)
{
	this->m_x = x;
}

void Bomb::setY(uint8_t y)
{
	this->m_y=y;
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
	int dx = static_cast<int>(m_x) - playerX;
	int dy = static_cast<int>(m_y) - playerY;
	return (dx * dx + dy * dy) <= (m_radius * m_radius);
}
