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

Direction::Direction():m_currentDirection(DirectionType::Up)
{
}
