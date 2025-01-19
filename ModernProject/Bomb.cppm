
export module Bomb;
import <chrono>;
import <memory>;
import "GameMap.h";
import "GameObject.h";
import <cstdint>;

export class Bomb : public GameObject {
public:
    Bomb();

    void SetCoordinates(GameMap& gameMap);
};
