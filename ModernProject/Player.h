#pragma once
#include<iostream>
#include<string_view>
#include<cstdint>
class Player
{
public:
	Player(const std::string_view& name, const std::pair<uint16_t, uint16_t>location);

private:
	std::string_view m_name;
	std::pair<uint16_t, uint16_t>m_location;
};