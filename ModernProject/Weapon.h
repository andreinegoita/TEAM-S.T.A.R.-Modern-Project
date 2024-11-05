#pragma once
#include<chrono>
class Weapon
{
public:
	Weapon();
	Weapon(double x, double y, double fireRate, double bulletSpeed, uint8_t direction);
	bool fire();
	void upgradeBulletSpeed(double increment);
	void reduceFireRate(double factor);
	double getFireRate() const;
	double getBulletSpeed()const;
private:
	double m_x,m_y;
	double m_fireRate;
	double m_bulletSpeed;
	uint8_t m_direction;
	std::chrono::time_point<std::chrono::steady_clock> m_lastFire;
};