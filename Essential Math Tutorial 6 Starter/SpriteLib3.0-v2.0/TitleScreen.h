#pragma once
#include "Scene.h"

class TitleScreen : public Scene
{
public:
	TitleScreen(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
};

