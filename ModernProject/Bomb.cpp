#include "Bomb.h"
#include<cmath>

Bomb::Bomb(uint8_t x, uint8_t y, uint16_t radius, bool isArmed):
	m_x(x),m_y(y),m_radius(radius),m_isArmed(isArmed)
{}

uint8_t Bomb::getX()
{
    return m_x;
}

uint8_t Bomb::getY()
{
	return m_y;
}

uint16_t Bomb::getRadius()
{
	return m_radius;
}

bool Bomb::getIsArmed()
{
	return m_isArmed;
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

void Bomb::setIsArmed(bool isArmed)
{
	this->m_isArmed = isArmed;
}

bool Bomb::isWithinRange(int playerX, int playerY) const
{
	int dx = playerX - m_x;
	int dy = playerY - m_y;
	return (std::sqrt(dx * dx + dy * dy) <= m_radius);
}
