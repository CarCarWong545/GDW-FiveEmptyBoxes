#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include "timer.h"


class FortuneLevel :
	public Scene
{
public:
	FortuneLevel(std::string name);

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

	bool isstunned = false;

	clock_t startstuntime;
	clock_t startyawntime;
	bool isyawn = false;
	bool isyawn2 = false;
	int vacuum = 0;
	bool ghost_1;
	int loop_anim = 0;

	int book = 0;

};
