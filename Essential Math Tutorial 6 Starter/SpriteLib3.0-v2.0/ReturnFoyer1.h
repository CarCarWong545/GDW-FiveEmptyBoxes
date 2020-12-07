#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class ReturnFoyer1 : public Scene
{
public:
	ReturnFoyer1(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;

	int ball = 0;

	bool dialogue = true;

	clock_t dialoguestart;

	double dialoguestop;

	bool canmove = true;

	unsigned int d;
};
