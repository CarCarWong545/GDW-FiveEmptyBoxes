#include "SceneTrigger.h"
#include "ECS.h"

void SceneTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void SceneTrigger::OnEnter()
{
	Trigger::OnEnter();
	if (s == 0)
	{
		auto& object = ECS::GetComponent<SwitchScene0>(m_targetEntities[0]);
		object.m_switch = true;
	}
	if (s == 1)
	{
		auto& object = ECS::GetComponent<SwitchScene>(m_targetEntities[0]);
		object.m_switch = true;
	}
	else if (s == 2)
	{
		auto& object = ECS::GetComponent<SwitchScene2>(m_targetEntities[0]);
		object.m_switch = true;
	}
	else if (s == 3)
	{
		auto& object = ECS::GetComponent<SwitchScene3>(m_targetEntities[0]);
		object.m_switch = true;
	}
}

void SceneTrigger::OnExit()
{
	Trigger::OnExit();
	if (s == 0)
	{
		auto& object = ECS::GetComponent<SwitchScene0>(m_targetEntities[0]);
		object.m_switch = false;
	}
	if (s == 1)
	{
		auto& object = ECS::GetComponent<SwitchScene>(m_targetEntities[0]);
		object.m_switch = false;
	}
	else if (s == 2)
	{
		auto& object = ECS::GetComponent<SwitchScene2>(m_targetEntities[0]);
		object.m_switch = false;
	}
	else if (s == 3)
	{
		auto& object = ECS::GetComponent<SwitchScene3>(m_targetEntities[0]);
		object.m_switch = false;
	}
}
SceneTrigger::SceneTrigger(int scene)
{
	s = scene;
}