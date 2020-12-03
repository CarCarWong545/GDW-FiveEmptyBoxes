#pragma once
#include "Trigger.h"
class FlashlightTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter(int entity) override;

	//void OnEnter() override;
	void OnExit() override;


protected:
	bool triggered = false;
};

