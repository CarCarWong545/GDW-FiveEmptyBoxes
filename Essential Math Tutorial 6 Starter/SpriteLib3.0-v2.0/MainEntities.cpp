#include "MainEntities.h"
#include <iostream>
#include <cstdlib>

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
int MainEntities::health = 100;
int MainEntities::m_flashlight = 0;

int MainEntities::MainCamera()
{
	return m_mainCamera;
}

int MainEntities::MainPlayer()
{
	return m_mainPlayer;
}

int MainEntities::Health()
{
	return health;
}

int MainEntities::Flashlight()
{
	return m_flashlight;
}

void MainEntities::MainCamera(int main)
{
	m_mainCamera = main;
}

void MainEntities::MainPlayer(int main)
{
	m_mainPlayer = main;
}

void MainEntities::Health(int n)
{
	health = n;
	if (health > 100)
	{
		health = 100;
	}
	if (health <= 0)
	{
		//end game somehow
		//std::exit(0);
	}
}

void MainEntities::Flashlight(int main)
{
	m_flashlight = main;
}
