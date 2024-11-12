#include "Bullet.h"

Bullet::Bullet(double x, double y, uint8_t direction, double speed, bool active):
	GameObject({x,y},speed), m_direction(direction), m_active(active)
{}


uint8_t Bullet::getDirection() const
{
	return m_direction;
}


bool Bullet::isActive() const
{
	return m_active;
}

void Bullet::deactivate()
{
	m_active = false;
}

void Bullet::Display()
{
	std::cout << 0;
}

void Bullet::Draw()
{
}



