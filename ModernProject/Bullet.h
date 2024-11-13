#pragma once
#include<cstdint>
#include "GameObject.h"
class Bullet:public GameObject {
public:
    Bullet(double, double, DirectionType,double, bool);
    DirectionType getDirection() const;
    bool isActive() const;
    void deactivate();
    void Display() override;
    void Draw() override;
    bool collide(const GameObject& other);

private:
    DirectionType m_direction;
    bool m_active;

};