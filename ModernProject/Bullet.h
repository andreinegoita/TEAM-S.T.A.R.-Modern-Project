#pragma once
#include<cstdint>
#include "GameObject.h"
#include "GameMap.h"
class Bullet:public GameObject {
public:
    Bullet(uint16_t, uint16_t, DirectionType,double, bool);
    DirectionType GetDirection() const;
    bool IsActive() const;
    void Deactivate();
    bool Collide(const GameObject& other);
    void MoveAndCheck(GameMap& gameMap);
    void UpdatePosition(const std::pair<uint16_t, uint16_t>& offset);
    void HandleOutOfBounds(GameMap& gameMap);
    void HandleCellInteraction(GameMap& gameMap);
    void StopAtWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY);
    void DestroyBreakableWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY);
    void MoveBullet(GameMap& gameMap, uint16_t oldX, uint16_t oldY);
    void DestroyNearbyWalls(GameMap& gameMap, uint16_t x, uint16_t y);
    void DestroyPlayer(GameMap& gameMap, uint16_t x, uint16_t y);
    void CheckBulletCollision(GameMap& gameMap, uint16_t x, uint16_t y);
private:
    DirectionType m_direction;
    bool m_active;

};