#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include "timer.h"

class NurseryLevel : public Scene
{
public:
	NurseryLevel(std::string name);

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

	bool canmove = true;

	bool firstdialogue = true;
	clock_t firstdstart;
	bool firstddelete = true;

	bool seconddialogue = true;
	clock_t secondstart;
	bool seconddelete = true;

	double firststop;
	double secondstop;

	unsigned int dialouge;
	unsigned int sdialouge;



	bool isstunned = false;

	clock_t startstuntime;
	int vacuum = 0;
	bool ghost_1;
	bool activate_ghost = false;
	int ghostdialogue = 0;
	int ball = 0;
	int horse = 0;
	bool second_phase = false;
	int loop_anim = 1;
};




