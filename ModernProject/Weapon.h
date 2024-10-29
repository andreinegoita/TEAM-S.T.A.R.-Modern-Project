#pragma once
#include<chrono>
class Weapon
{
public:
	Weapon();
	bool fire();
	void upgradeBulletSpeed(double increment);
	void reduceFireRate(double factor);
	double getFireRate() const;
	double getBulletSpeed()const;
private:
	double m_fireRate;
	double m_bulletSpeed;
	std::chrono::time_point<std::chrono::steady_clock> m_lastFire;
};