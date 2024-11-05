#pragma once

#include<iostream>

class GameObject
{

public:
	GameObject(std::pair<uint16_t, uint16_t>position, float velocity);
	virtual void Display() = 0;
	~GameObject() = default;
protected:
	std::pair<uint16_t, uint16_t>m_position;
	float m_velocity;
};
