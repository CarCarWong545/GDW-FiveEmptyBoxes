#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class HallLevel2OutofDining :public Scene
{
public:
	HallLevel2OutofDining(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;

};