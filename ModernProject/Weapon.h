#pragma once
#include<chrono>
class Weapon
{
private:
	double m_fireRate;
	double m_bulletSpeed;
	std::chrono::time_point<std::chrono::steady_clock> m_lastFire;
public:
	Weapon();
};

