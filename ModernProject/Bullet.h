#pragma once
#include<cstdint>
#include "GameObject.h"
class Bullet:public GameObject {
public:
    Bullet(double, double, uint8_t, double, bool);
    uint8_t getDirection() const;
    bool isActive() const;
    void deactivate();
private:
    uint8_t m_direction;
    bool m_active;

};