#pragma once
#include "Trigger.h"
class VTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	VTrigger(int n);

	void OnEnter() override;
	void OnExit() override;
private:
	int enemy; //according to enemy index
};

