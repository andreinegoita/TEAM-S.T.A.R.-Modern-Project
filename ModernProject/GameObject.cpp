#include "GameObject.h"

GameObject::GameObject(std::pair<uint16_t, uint16_t>position, double speed, DirectionType direction) :m_position{ position }, m_speed{ speed }, m_direction{direction}
{
}

std::pair<uint16_t, uint16_t> GameObject::GetPosition() const
{
	return m_position;
}




