#pragma once

#include<iostream>
#include "Direction.h"
#include"GameMap.h"
#include"Color.h"

class GameObject
{

public:
	GameObject(std::pair<uint16_t, uint16_t>position, double speed, DirectionType direction);
	virtual void Display() = 0;
	virtual ~GameObject() = default;
	virtual void Draw() = 0;

	void printColored(char character, Color color);

	std::pair<uint16_t, uint16_t>GetPosition() const;
	void move(GameMap& gameMap);
	bool isOutOfBounds(const GameMap& gameMap,uint16_t first, uint16_t second);

protected:
	DirectionType m_direction;
	std::pair<uint16_t, uint16_t>m_position;
	double m_speed;

	std::pair<int, int> getMovementOffset() const;
};
