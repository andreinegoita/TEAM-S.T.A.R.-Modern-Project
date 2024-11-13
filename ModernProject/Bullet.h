#pragma once
#include<cstdint>
#include "GameObject.h"
#include "GameMap.h"
class Bullet:public GameObject {
public:
    Bullet(double, double, DirectionType,double, bool);
    DirectionType getDirection() const;
    bool isActive() const;
    void deactivate();
    void Display() override;
    void Draw() override;
    bool collide(const GameObject& other);
    void moveAndCheck(const GameMap& gameMap, std::vector<GameObject*>& objects);
private:
    DirectionType m_direction;
    bool m_active;

};