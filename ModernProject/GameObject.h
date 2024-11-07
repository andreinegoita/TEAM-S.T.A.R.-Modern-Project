#pragma once

#include<iostream>

class GameObject
{

public:
	GameObject(std::pair<uint16_t, uint16_t>position, double speed);
	virtual void Display() = 0;
	virtual ~GameObject() = default;
	uint16_t GetX();
	uint16_t GetY();
	double GetSpeed();

	


protected:
	std::pair<uint16_t, uint16_t>m_position;
	double m_speed;
};
