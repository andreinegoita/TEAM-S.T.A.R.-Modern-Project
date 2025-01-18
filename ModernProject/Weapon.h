//#pragma once
//#include<chrono>
//#include"Bullet.h"
//#include<memory>
//#include"GameObject.h"
//class Weapon: public GameObject
//{
//public:
//	Weapon();
//	Weapon(uint16_t x, uint16_t y, double fireRate, double bulletSpeed, DirectionType direction);
//	std::unique_ptr<Bullet>Fire(uint16_t x, uint16_t y, DirectionType direction);
//	void UpgradeBulletSpeed(double increment);
//	void ReduceFireRate(double factor);
//	double GetFireRate() const;
//	double GetBulletSpeed()const;
//	void SetBulletSpeed(double speed);
//private:
//	double m_fireRate;
//	double m_bulletSpeed;
//	std::chrono::time_point<std::chrono::steady_clock> m_lastFire;
//};