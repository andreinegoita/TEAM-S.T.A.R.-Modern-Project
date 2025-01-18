// Bomb.cppm
export module Bomb;
import <chrono>;
import <memory>;
import "GameMap.h";
import "GameObject.h";
import <cstdint>;

export class Bomb : public GameObject {
public:
    Bomb(); // Constructor

    // Metodă pentru a seta coordonatele bombei pe harta de joc
    void SetCoordinates(GameMap& gameMap);
};
