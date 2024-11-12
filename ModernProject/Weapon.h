#pragma once
#include<chrono>
#include"Bullet.h"
#include<memory>
class Weapon: public GameObject
{
public:
	Weapon();
	Weapon(double x, double y, double fireRate, double bulletSpeed, uint8_t direction);
	std::unique_ptr<Bullet> fire(uint16_t x, uint16_t y, uint8_t direction);
	void upgradeBulletSpeed(double increment);
	void reduceFireRate(double factor);
	double getFireRate() const;
	double getBulletSpeed()const;
	void Display() override;
	void Draw() override;
private:
	double m_fireRate;
	double m_bulletSpeed;
	uint8_t m_direction;
	std::chrono::time_point<std::chrono::steady_clock> m_lastFire;
};