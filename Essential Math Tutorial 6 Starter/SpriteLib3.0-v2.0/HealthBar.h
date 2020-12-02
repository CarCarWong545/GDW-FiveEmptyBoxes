#pragma once
#include <vector>
class HealthBar
{
public:
	void UpdateHealthBar(int healthBar, int healthBackBar);
	void UpdateGhostCounter(std::vector<int> ghosts, int fillColour, int backColour);
	void UpdateVignette();
};

