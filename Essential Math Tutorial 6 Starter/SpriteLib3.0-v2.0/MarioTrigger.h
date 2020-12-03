#pragma once
#include "Trigger.h"
class MarioTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter(int entity) override;

	//void OnEnter() override;
	void OnExit() override;
};

