#pragma once
#include<iostream>
#include<string_view>
#include<cstdint>
class Player
{
public:
	Player( std::string_view name,std::pair<uint16_t, uint16_t>location,uint16_t health,double movespeed);
	void Shoot();
	// Method which player shooting other players
	/// </summary>
	void ResetPosition();
	//Reset position from startPoint
private:
	std::string_view m_name;
	std::pair<uint16_t, uint16_t>m_location;
	std::uint16_t m_health;
	double m_movespeed;
	int points;
	int score;

};