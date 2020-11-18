#include "SceneTrigger.h"
#include "ECS.h"
#include "Player.h"

void SceneTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void SceneTrigger::OnEnter()
{
	Trigger::OnEnter();
	auto& canDoor = ECS::GetComponent<CanDoor>(m_targetEntities[0]);
	auto& player = ECS::GetComponent<Player>(m_targetEntities[0]);
	if (player.m_equip)
	{
		canDoor.m_door = true;
		if (s == 0)
		{
			auto& object = ECS::GetComponent<SwitchScene0>(m_targetEntities[0]);
			object.can_switch = true;
		}
		if (s == 1)
		{
			auto& object = ECS::GetComponent<SwitchScene>(m_targetEntities[0]);
			object.can_switch = true;
		}
		else if (s == 2)
		{
			auto& object = ECS::GetComponent<SwitchScene2>(m_targetEntities[0]);
			object.can_switch = true;
		}
		else if (s == 3)
		{
			auto& object = ECS::GetComponent<SwitchScene3>(m_targetEntities[0]);
			object.can_switch = true;
		}
	}
}

void SceneTrigger::OnExit()
{
	Trigger::OnExit();
	auto& canDoor = ECS::GetComponent<CanDoor>(m_targetEntities[0]);
	canDoor.m_door = false;

	if (s == 0)
	{
		auto& object = ECS::GetComponent<SwitchScene0>(m_targetEntities[0]);
		object.can_switch = false;
	}
	if (s == 1)
	{
		auto& object = ECS::GetComponent<SwitchScene>(m_targetEntities[0]);
		object.can_switch = false;
	}
	else if (s == 2)
	{
		auto& object = ECS::GetComponent<SwitchScene2>(m_targetEntities[0]);
		object.can_switch = false;
	}
	else if (s == 3)
	{
		auto& object = ECS::GetComponent<SwitchScene3>(m_targetEntities[0]);
		object.can_switch = false;
	}
}
SceneTrigger::SceneTrigger(int scene)
{
	s = scene;
}
