#pragma once
#include<cstdint>

class Bomb {
public:
    Bomb(uint8_t, uint8_t, uint16_t, bool);
    uint8_t getX();
    uint8_t getY();
    uint16_t getRadius();
    bool getIsArmed();
    void setX(uint8_t);
    void setY(uint8_t);
    void setRadius(uint16_t);
    void setIsArmed(bool);
    bool isWithinRange(int, int) const;

private:
    uint8_t m_x, m_y;
    uint16_t m_radius;
    bool m_isArmed;
    const uint8_t m_DETONATION_RADIUS = 10;
};