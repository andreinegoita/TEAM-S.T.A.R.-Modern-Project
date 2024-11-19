#pragma once
#include<cstdint>
enum class CellType : uint16_t
{
	EMPTY,
	BREAKABLE_WALL,
	UNBREAKABLE_WALL,
	Player,
	Bullet
};

