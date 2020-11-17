#include "DialogueTrigger.h"
#include "ECS.h"
#include "Player.h"

void DialogueTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void DialogueTrigger::OnEnter()
{
	Trigger::OnEnter();
	auto& object = ECS::GetComponent<Player>(m_targetEntities[0]);
	object.m_equip = true;
}

void DialogueTrigger::OnExit()
{
	Trigger::OnExit();
}

