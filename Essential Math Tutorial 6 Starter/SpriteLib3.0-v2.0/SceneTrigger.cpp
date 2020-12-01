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
		else if (s == 4)
		{

			object.can_switch4 = true;
		}
		else if (s == 5)
		{

			object.can_switch5 = true;
		}
		else if (s == 6)
		{

			object.can_switch6 = true;
		}
		else if (s == 7)
		{

			object.can_switch7 = true;
		}
		else if (s == 8)
		{

			object.can_switch8 = true;
		}
		else if (s == 9)
		{

			object.can_switch9 = true;
		}
		else if (s == 10)
		{

			object.can_switch10 = true;
		}
		else if (s == 11)
		{

			object.can_switch11 = true;
		}
		else if (s == 12)
		{

			object.can_switch12 = true;
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
	else if (s == 4)
	{

		object.can_switch4 = false;
	}
	else if (s == 5)
	{

		object.can_switch5 = false;
	}
	else if (s == 6)
	{

		object.can_switch6 = false;
	}
	else if (s == 7)
	{

		object.can_switch7 = false;
	}
	else if (s == 8)
	{

		object.can_switch8 = false;
	}
	else if (s == 9)
	{

		object.can_switch9 = false;
	}
	else if (s == 10)
	{

		object.can_switch10 = false;
	}
	else if (s == 11)
	{

		object.can_switch11 = false;
	}
	else if (s == 12)
	{

		object.can_switch12 = false;
	}
}
SceneTrigger::SceneTrigger(int scene)
{
	s = scene;
}
