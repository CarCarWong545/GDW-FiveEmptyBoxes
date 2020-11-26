#pragma once
#include "Trigger.h"
class VTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
};

