#pragma once
#include "Trigger.h"
class MoveDownTrigger :
	public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
};

