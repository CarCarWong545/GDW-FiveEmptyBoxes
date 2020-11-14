#include "MoveUpTrigger.h"
#include "ECS.h"

void MoveUpTrigger::OnTrigger()
{
	Trigger::OnTrigger();
	auto& object = ECS::GetComponent<MoveUp>(m_targetEntities[0]);
	object.moveUp = true;
}

void MoveUpTrigger::OnEnter()
{
	Trigger::OnEnter();
	auto& object = ECS::GetComponent<MoveUp>(m_targetEntities[0]);
	object.moveUp = true;
}

void MoveUpTrigger::OnExit()
{
	Trigger::OnExit();
	auto& object = ECS::GetComponent<MoveUp>(m_targetEntities[0]);
	object.moveUp = false;
}
