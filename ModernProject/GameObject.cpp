#include "GameObject.h"

GameObject::GameObject(std::pair<uint16_t, uint16_t>position, double speed, DirectionType direction) :m_position{ position }, m_speed{ speed }, m_direction{direction}
{
}

std::pair<uint16_t, uint16_t> GameObject::GetPosition() const
{
	return m_position;
}

void GameObject::move()
{
    std::pair<int, int> offset = getMovementOffset();

    m_position.first += static_cast<uint16_t>(offset.first * m_speed);
    m_position.second += static_cast<uint16_t>(offset.second * m_speed);
}

bool GameObject::isOutOfBounds(const GameMap& gameMap)
{
    const auto [rows, cols] = std::make_pair(gameMap.getRows(), gameMap.getCols());

    if (m_position.first >= rows || m_position.second >= cols 
        || m_position.first < 0 || m_position.second < 0) {
        return true;
    }
    return false;
}




