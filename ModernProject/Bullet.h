#pragma once
#include<cstdint>

class Bullet {
public:
    Bullet(double, double, uint8_t, double, bool);
    double getX();
    double getY();
    uint8_t getDirection();
    double getSpeed();
    void setX(double);
    void setY(double);
    void setDirection(double);
    void setSpeed(double);
private:
    double m_x, m_y;
    uint8_t m_direction;
    double m_speed;
    bool m_active;

};