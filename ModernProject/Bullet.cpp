#include "Bullet.h"

Bullet::Bullet(double x, double y, uint8_t direction, double speed, bool active):
	m_x(x),m_y(y),m_direction(direction),m_speed(speed),m_active(active)
{}

double Bullet::getX() const
{
	return m_x;
}

double Bullet::getY() const
{
	return m_y;
}

uint8_t Bullet::getDirection() const
{
	return m_direction;
}

double Bullet::getSpeed() const
{
	return m_speed;
}

void Bullet::setX(double x)
{
	this->m_x = x;
}

void Bullet::setY(double y)
{
	this->m_y = y;
}



