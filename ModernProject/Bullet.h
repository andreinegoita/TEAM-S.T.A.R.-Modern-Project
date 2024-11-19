#pragma once
#include<cstdint>
#include "GameObject.h"
#include "GameMap.h"
class Bullet:public GameObject {
public:
    Bullet(uint16_t, uint16_t, DirectionType,double, bool);
    DirectionType getDirection() const;
    bool isActive() const;
    void deactivate();
    void Display() override;
    void Draw() override;
    bool collide(const GameObject& other);
    void moveAndCheck(GameMap& gameMap, std::vector<GameObject*>& objects);
    void updatePosition(const std::pair<uint16_t, uint16_t>& offset);
    void handleOutOfBounds(GameMap& gameMap);
    void handleCellInteraction(GameMap& gameMap);
    void stopAtWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY);
    void destroyBreakableWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY);
    void moveBullet(GameMap& gameMap, uint16_t oldX, uint16_t oldY);
private:
    DirectionType m_direction;
    bool m_active;

};