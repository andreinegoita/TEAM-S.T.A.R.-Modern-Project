#pragma once
#include<cstdint>
#include"GameObject.h"
class Bomb: public GameObject {
public:
    Bomb(uint16_t, uint16_t, uint16_t, bool);
    uint16_t getRadius() const;
    bool getIsActivated() const;
    void setRadius(uint16_t);
    void setIsArmed(bool);
    bool isWithinRange(int, int) const;
    uint8_t GetRadius() const;
    void setCoordinates(GameMap& gameMap);
private:
    uint16_t m_radius;
    bool m_isActivated;
    const uint8_t m_DETONATION_RADIUS = 10;
};