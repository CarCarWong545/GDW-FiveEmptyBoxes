#include "VTrigger.h"
#include "ECS.h"
#include "Player.h"

void VTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void VTrigger::OnEnter()
{
	Trigger::OnEnter();
	if (ECS::GetComponent<Player>(m_targetEntities[1]).m_suck)
	{
		auto& body = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]);
		auto& ghost = ECS::GetComponent<CanDamage>(m_targetEntities[0]);
		if (ghost.m_stun)
		{
			ghost.m_candamage = false;
			ghost.m_suck = true;

		}
	}
}

void VTrigger::OnExit()
{
	Trigger::OnExit();
	auto& body = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]);
	auto& ghost = ECS::GetComponent<CanDamage>(m_targetEntities[0]);
	ghost.m_suck = false;

}
