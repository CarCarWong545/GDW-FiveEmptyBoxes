#pragma once
#include "Trigger.h"
class SceneTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
	SceneTrigger(int scene);
private:
	int s;
};

