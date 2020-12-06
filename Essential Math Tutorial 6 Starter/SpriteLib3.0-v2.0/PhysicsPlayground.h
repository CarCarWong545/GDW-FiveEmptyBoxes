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

	static void changeFlashlight(bool on);

protected:
	PhysicsPlaygroundListener listener;
	bool firstdialogue = true;
	bool secondd = true;
	bool thirdd = true;
	bool canmove = true;

	clock_t firstdstart;
	clock_t seconddstart;
	clock_t thirddstart;
	
	
	double firstdstop;
	double secondstop;
	double thirdstop;
	
	const double secondspassforfirst = 3;

	bool deletefirstd = true;
	bool deletesecondd = true;
	bool DialoguedoneEGadd = false;

	unsigned int fd;
	unsigned int sd;
	unsigned int td;

};
