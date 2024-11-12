#include <iostream>

enum class DirectionType {
    Up,
    Down,
    Left,
    Right
};

class Direction {
public:
    DirectionType m_currentDirection;
    Direction();
    void updateDirection(char key);
    std::pair<int, int> getMovementOffset() const;
};

