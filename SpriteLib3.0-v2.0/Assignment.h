#pragma once
#include "Scene.h";

class Assignment : public Scene
{
public:
	Assignment(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//void Unload();

	//void AdjustScrollOffset();


	//were overriding the already standard Scene input methods
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
protected:
};
