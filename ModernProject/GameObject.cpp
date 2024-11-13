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

void GameObject::move(const GameMap& gameMap)
{   
        std::pair<int, int> offset = getMovementOffset();
    if (!isOutOfBounds(gameMap,m_position.first+offset.first,m_position.second+offset.second))
    {
        m_position.first += offset.first * m_speed;
        m_position.second += offset.second * m_speed;
    }
}




