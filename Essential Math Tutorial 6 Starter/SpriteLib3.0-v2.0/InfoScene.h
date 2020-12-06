#pragma once
#include "Scene.h"

class InfoScene : public Scene
{
public:
	InfoScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	void KeyboardHold() override;

};
