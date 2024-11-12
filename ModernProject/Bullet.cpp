#include "Bullet.h"

Bullet::Bullet(double x, double y, DirectionType direction, double speed, bool active):
	GameObject({x,y},speed,direction), m_direction(direction), m_active(active)
{}


DirectionType Bullet::getDirection() const
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



