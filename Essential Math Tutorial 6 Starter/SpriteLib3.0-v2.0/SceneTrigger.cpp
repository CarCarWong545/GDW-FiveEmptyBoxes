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
		auto& object = ECS::GetComponent<SwitchScene>(m_targetEntities[0]);
		canDoor.m_door = true;
		if (s == 0)
		{
			
			object.can_switch0 = true;
		}
		if (s == 1)
		{
			
			object.can_switch1 = true;
		}
		else if (s == 2)
		{
			
			object.can_switch2 = true;
		}
		else if (s == 3)
		{
			
			object.can_switch3 = true;
		}
	}
}

void SceneTrigger::OnExit()
{
	Trigger::OnExit();
	auto& canDoor = ECS::GetComponent<CanDoor>(m_targetEntities[0]);
	canDoor.m_door = false;
	auto& object = ECS::GetComponent<SwitchScene>(m_targetEntities[0]);
	if (s == 0)
	{
		
		object.can_switch0 = false;
	}
	if (s == 1)
	{
		
		object.can_switch1 = false;
	}
	else if (s == 2)
	{
		
		object.can_switch2 = false;
	}
	else if (s == 3)
	{
		
		object.can_switch3 = false;
	}
}
SceneTrigger::SceneTrigger(int scene)
{
	s = scene;
}
