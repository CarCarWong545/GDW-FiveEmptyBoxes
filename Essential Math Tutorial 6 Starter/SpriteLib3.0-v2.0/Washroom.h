#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include "timer.h"


class Washroom :
	public Scene
{
public:
	Washroom(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;

	int ghost1 = 0;
	int ghost2 = 0; //ghost 1 physics body
	int flashlight = 0;

	float elapsedtime = 0.f;
	float stuntime = 3.f;

	clock_t startstuntime;
	clock_t startyawntime;
	bool isyawn = false;
	bool isyawn2 = false;
	int vacuum = 0;
	bool ghost_1;
	int loop_anim = 0;

	clock_t startstuntime2;
	bool ghost_21 = false;
	int ghost21 = 0;
	int ghost22 = 0;

	clock_t startstuntime3;
	bool ghost_31 = false;
	int ghost31 = 0;
	int ghost32 = 0;

	bool isstunned = false;
};