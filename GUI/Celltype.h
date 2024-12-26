#pragma once
#include<cstdint>
#include<string>
enum class CellType : uint16_t
{
	EMPTY,
	BREAKABLE_WALL,
	UNBREAKABLE_WALL,
	Player,
	Bullet,
	Bomb
};

