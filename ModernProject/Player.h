#pragma once
#include<iostream>
#include<string_view>
#include<cstdint>
#include"GameObject.h"

class Player:public GameObject
{
public:
	Player( std::string_view name,std::pair<uint16_t,uint16_t>position,float velocity);
	void Shoot();
	// Method which a players shooting another players;
	void ResetPosition();
	// Method which player respawn in startPoint;
	void Display() override;
private:
	std::string_view m_name;
	std::uint16_t m_health;
	int m_score;
	int m_points;
};