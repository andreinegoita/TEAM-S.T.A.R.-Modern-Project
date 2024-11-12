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

//bool Bullet::collide(const GameObject& other)
//{
//	auto bulletPosition = this->GetPosition();
//	auto objectPosition = other.GetPosition();
//
//	double distance = std::sqrt(std::pow(bulletPosition.first - objectPosition.first, 2) +
//		std::pow(bulletPosition.second - objectPosition.second, 2));
//}
//
//	//double bulletRadius = this->GetRadius();
//	//double objectRadius = other.getRadius();
//
//	//const double collisionThreshold = bulletRadius + objectRadius;;
//	/*if (distance < collisionThreshold) {
//		return true;
//	}
//
//	return false;
//}*/





