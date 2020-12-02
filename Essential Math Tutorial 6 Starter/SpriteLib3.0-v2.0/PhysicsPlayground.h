#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include "timer.h"


class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	
	static void changeFlashlight(bool);
protected:
	PhysicsPlaygroundListener listener;
	bool firstdialogue = true;
	bool secondd = true;
	bool thirdd = true;

	clock_t firstdstart;
	
	
	double firstdstop;
	
	const double secondspassforfirst = 3;

	bool deletefirstd = true;

	unsigned int fd;
	unsigned int sd;
};
