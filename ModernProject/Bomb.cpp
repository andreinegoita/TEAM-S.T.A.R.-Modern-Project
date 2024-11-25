#include "Bomb.h"
#include<cmath>

Bomb::Bomb(uint16_t x, uint16_t y, uint16_t radius, bool isActivated):
	GameObject({ x,y },0,DirectionType::Up), m_radius(radius), m_isActivated(isActivated)
{}


uint16_t Bomb::getRadius() const
{
	return m_radius;
}

bool Bomb::getIsActivated() const
{
	return m_isActivated;
}


void Bomb::setRadius(uint16_t radius)
{
	this->m_radius = radius;
}

void Bomb::setIsArmed(bool isActivated)
{
	this->m_isActivated = isActivated;
}

bool Bomb::isWithinRange(int playerX, int playerY) const
{
	int dx = static_cast<int>(m_position.first) - playerX;
	int dy = static_cast<int>(m_position.second) - playerY;
	return (dx * dx + dy * dy) <= (m_radius * m_radius);
}



uint8_t Bomb::GetRadius() const
{
	return m_DETONATION_RADIUS;
}

void Bomb::setCoordinates(GameMap& gameMap)
{
	uint16_t sizeRows = gameMap.getRows(), sizeCols = gameMap.getCols();
	std::mt19937 mt(time(nullptr));
	uint16_t randomRow = mt() % sizeRows;
	uint16_t randomCol = mt() % sizeCols;
	while (true)
	{
		uint16_t randomRow = mt() % sizeRows;
		uint16_t randomCol = mt() % sizeCols;
		if (gameMap.getCellType(randomRow, randomCol) == CellType::BREAKABLE_WALL)
		{
			this->m_position.first = randomRow;
			this->m_position.second = randomCol;
			gameMap.setCellType(m_position.first, m_position.second, CellType::Bomb);
			break;
		}
	}
}
