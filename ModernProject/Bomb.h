#pragma once
#include<cstdint>
#include"GameObject.h"

class Bomb: public GameObject {

public:
    Bomb();
    void setCoordinates(GameMap& gameMap);
};