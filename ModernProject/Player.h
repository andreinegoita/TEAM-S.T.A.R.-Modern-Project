#pragma once
#include<iostream>
#include<string_view>
#include<cstdint>
class Player
{
public:
	Player( std::string_view name,std::pair<uint16_t, uint16_t>location,uint16_t health,double movespeed);
	void Shoot();
	// Method which a players shooting another players;
	void ResetPosition();
	// Method which player respawn in startPoint;
private:
	std::string_view m_name;
	std::pair<uint16_t, uint16_t>m_location;
	std::uint16_t m_health;
	double m_movespeed;
	int m_score;
	int m_points;
};