#pragma once
#include "Trigger.h"
class DialogueTrigger :
    public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
};

