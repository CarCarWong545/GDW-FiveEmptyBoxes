#pragma once
#include "Trigger.h"
class MoveUpTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
};

