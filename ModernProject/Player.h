#pragma once
#include<iostream>
#include<string_view>
#include<cstdint>
#include <conio.h>
#include"GameMap.h"
#include"GameObject.h"
#include"Weapon.h"

class Player:public GameObject
{
public:
	Player( std::string_view name,std::pair<uint16_t,uint16_t>position,double velocity,DirectionType direction);
	void Shoot();

	void ResetPosition();

	std::pair<uint16_t, uint16_t>GetStartPosition();

	void HandleInput(GameMap& gameMap);

	void UpdateBullets(GameMap& gameMap);

private:
	std::chrono::steady_clock::time_point m_lastShootTime;
	std::string_view m_name;
	std::uint16_t m_health;
	int m_score;
	int m_points;
	Weapon m_weapon;
	std::vector<std::unique_ptr<Bullet>> m_bullets;
	std::pair<uint16_t, uint16_t>m_startPosition;
};