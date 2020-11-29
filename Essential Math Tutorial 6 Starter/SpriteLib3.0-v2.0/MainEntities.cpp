#include "MainEntities.h"
#include <iostream>
#include <cstdlib>
#include "ECS.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
int MainEntities::health = 100;
int MainEntities::m_flashlight = 0;
int MainEntities::enemies[10] = { 1, 2, 3,4,5, 6, 7, 8, 9, 10};
//1 = blue ghost, 2= neville...0 = gone
int MainEntities::captured = 0;


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

int* MainEntities::Enemies()
{
	return enemies;
}

int MainEntities::Captured()
{
	return captured;
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
		//idk
	}
}

void MainEntities::Flashlight(int main)
{
	m_flashlight = main;
}

void MainEntities::Enemies(int a[10])
{
	for (int i = 0; i < 10; i++)
	{
		enemies[i] = a[i];
	}
}

void MainEntities::Capture(int n)
{
	captured = n;
}
