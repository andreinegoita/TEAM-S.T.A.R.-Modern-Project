#include "GameObject.h"
#include "GameMap.h"

GameObject::GameObject(std::pair<uint16_t, uint16_t>position, double speed, DirectionType direction) :m_position{ position }, m_speed{ speed }, m_direction{direction}
{
}

std::pair<uint16_t, uint16_t> GameObject::GetPosition() const
{
	return m_position;
}

bool GameObject::isOutOfBounds(const GameMap& gameMap,uint16_t first, uint16_t second)
{
    const auto [rows, cols] = std::make_pair(gameMap.getRows(), gameMap.getCols());
    if (first >= rows || second >= cols 
        || first < 0 || second < 0) {
        return true;
    }
    return false;
}

void GameObject::move(GameMap& gameMap)
{   
    int oldX = m_position.first;
    int oldY = m_position.second;

    std::pair<int,int>offset = getMovementOffset();
    int newX = m_position.first + offset.first;
    int newY = m_position.second + offset.second;
    if (isOutOfBounds(gameMap, newX, newY) && gameMap.getCellType(newX, newY) == CellType::EMPTY)
    {
        gameMap.setCellType(oldX, oldY, CellType::EMPTY);
        m_position.first = newX;
        m_position.second = newY;
        gameMap.setCellType(newX, newY, CellType::Player);
    }
}




