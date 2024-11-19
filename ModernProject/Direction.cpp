#include "Direction.h"


void Direction::updateDirection(char key) {
    switch (key) {
    case 'W': case 'w':
        m_currentDirection = DirectionType::Up;
        break;
    case 'A': case 'a':
        m_currentDirection = DirectionType::Left;
        break;
    case 'S': case 's':
        m_currentDirection = DirectionType::Down;
        break;
    case 'D': case 'd':
        m_currentDirection = DirectionType::Right;
        break;
    default:
        break;
    }
}

std::pair<int, int> Direction::getMovementOffset() const {
    switch (m_currentDirection) {
    case DirectionType::Up:    return { 0, -1 };
    case DirectionType::Down:  return { 0, 1 };
    case DirectionType::Left:  return { -1, 0 };
    case DirectionType::Right: return { 1, 0 };
    }
    return { 0, 0 };
}
Direction::Direction():m_currentDirection(DirectionType::Up)
{
}
