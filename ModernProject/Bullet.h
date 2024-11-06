#pragma once
#include<cstdint>

class Bullet {
public:
    Bullet(double, double, uint8_t, double, bool);
    double getX() const;
    double getY() const;
    uint8_t getDirection() const;
    double getSpeed() const;
    void setX(double);
    void setY(double);
private:
    double m_x, m_y;
    uint8_t m_direction;
    double m_speed;
    bool m_active;

};