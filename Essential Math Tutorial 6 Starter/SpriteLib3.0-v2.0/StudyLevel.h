#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include "timer.h"


class StudyLevel :
    public Scene
{
public:
	StudyLevel(std::string name);

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
	int ghost2 = 0;
	int flashlight = 0;

	float elapsedtime = 0.f;
	float stuntime = 3.f;

	bool isstunned = false;

	clock_t startstuntime;
	
};

