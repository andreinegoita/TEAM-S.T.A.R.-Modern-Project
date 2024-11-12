#pragma once

#include<iostream>

class GameObject
{

public:
	GameObject(std::pair<uint16_t, uint16_t>position, double speed);
	virtual void Display() = 0;
	virtual ~GameObject() = default;
	virtual void Draw() = 0;
	enum class Color
	{
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		DEFAULT = 0
	};

	void printColored(char character, Color color) {
		std::cout << "\033[" << static_cast<int>(color) << "m" << character << "\033[0m" << std::endl;
	}


protected:
	std::pair<uint16_t, uint16_t>m_position;
	double m_speed;
};
