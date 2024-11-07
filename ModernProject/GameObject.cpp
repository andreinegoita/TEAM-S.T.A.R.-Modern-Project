#include "GameObject.h"

GameObject::GameObject(std::pair<uint16_t,uint16_t>position,double speed):m_position{position},m_speed{speed}
{
}

uint16_t GameObject::GetX()
{
	return m_position.first;
}

uint16_t GameObject::GetY()
{
	return  m_position.second;
}

double GameObject::GetSpeed()
{
	return m_speed;
}



