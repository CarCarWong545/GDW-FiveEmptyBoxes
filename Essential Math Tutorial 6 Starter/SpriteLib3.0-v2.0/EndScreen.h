#pragma once
#include "Scene.h"
class EndScreen :public Scene
{
public:
	EndScreen(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	//void setSelect(int);
	int setX(int);
	int setY(int);
	int setW(int);
	int setH(int);
};
