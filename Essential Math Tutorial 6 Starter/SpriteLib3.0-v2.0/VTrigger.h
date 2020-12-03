#pragma once
#include "Trigger.h"
class VTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	VTrigger(std::vector<int>n);

	void OnEnter(int entity) override;

	//void OnEnter() override;
	void OnExit() override;
private:
	std::vector<int>enemy; //according to enemy index
};

