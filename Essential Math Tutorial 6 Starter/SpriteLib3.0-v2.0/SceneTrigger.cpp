#include "SceneTrigger.h"
#include "ECS.h"
#include "Player.h"

void SceneTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void SceneTrigger::OnEnter(int entity)
{
	Trigger::OnEnter(entity);
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
			auto enemies = MainEntities::Enemies();
			//if (enemies[0] == 0 && enemies[1] == 0 && enemies[2] == 0) //can only go into this scene if 3 enemies are defeated
			{
				object.can_switch6 = true;
			}
			//else
			{
				//object.can_switch6 = false;
			}
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
		else if (s == 13)
		{

			object.can_switch13 = true;
		}
		else if (s == 15)
		{

			object.can_switch15 = true;
		}
		else if (s == 16)
		{

			object.can_switch16 = true;
		}
		else if (s == 17)
		{

			object.can_switch17 = true;
		}
		else if (s == 18)
		{

			object.can_switch18 = true;
		}
		else if (s == 19)
		{

			object.can_switch19 = true;
		}
		else if (s == 20)
		{

			object.can_switch20 = true;
		}
		else if (s == 21)
		{

		object.can_switch21 = true;
		}
		else if (s == 22)
		{

		object.can_switch22 = true;
		}
		else if (s == 23)
		{

		object.can_switch23 = true;
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
	else if (s == 13)
	{

		object.can_switch13 = false;
	}
	else if (s == 15)
	{

		object.can_switch15 = false;
	}
	else if (s == 16)
	{

		object.can_switch16 = false;
	}
	else if (s == 17)
	{

		object.can_switch17 = false;
	}
	else if (s == 18)
	{

		object.can_switch18 = false;
	}
	else if (s == 19)
	{

		object.can_switch19 = false;
	}
	else if (s == 20)
	{

		object.can_switch20 = false;
	}
	else if (s == 21)
	{

		object.can_switch21 = false;
	}
	else if (s == 22)
	{

		object.can_switch22 = false;
	}
	else if (s == 23)
	{

		object.can_switch23 = false;
	}
}
SceneTrigger::SceneTrigger(int scene)
{
	s = scene;
}
