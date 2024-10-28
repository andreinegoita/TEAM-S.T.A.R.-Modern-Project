#include "Bullet.h"

Bullet::Bullet(double x, double y, uint8_t direction, double speed, bool active):
	m_x(x),m_y(y),m_direction(direction),m_speed(speed),m_active(active)
{}

double Bullet::getX()
{
	return m_x;
}

double Bullet::getY()
{
	return m_y;
}

uint8_t Bullet::getDirection()
{
	return m_direction;
}

double Bullet::getSpeed()
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

void Bullet::setDirection(double direction)
{
	this->m_direction = direction;
}

void Bullet::setSpeed(double speed)
{
	this->m_speed = speed;
}
