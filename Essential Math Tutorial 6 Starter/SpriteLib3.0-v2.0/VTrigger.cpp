#include "VTrigger.h"
#include "ECS.h"
#include "Player.h"

void VTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}
VTrigger::VTrigger(int n)
{
	enemy = n;
}

void VTrigger::OnEnter()
{
	Trigger::OnEnter();
	if (ECS::GetComponent<Player>(m_targetEntities[0]).m_suck)
	{
		for (int i = 1; i < m_targetEntities.size(); i++)
		{
			auto& body = ECS::GetComponent<PhysicsBody>(m_targetEntities[i]);
			auto& ghost = ECS::GetComponent<CanDamage>(m_targetEntities[i]);
			if (!ghost.m_candamage)
			{
				ghost.m_stun = true;
				ghost.m_suck = true;

			}
		}
	}
}

void VTrigger::OnExit()
{
	Trigger::OnExit();
	int* enemies = MainEntities::Enemies();
	for (int i = 1; i < m_targetEntities.size(); i++)
	{
		if (enemies[enemy] != 0)
		{
			auto& body = ECS::GetComponent<PhysicsBody>(m_targetEntities[i]);
			auto& ghost = ECS::GetComponent<CanDamage>(m_targetEntities[i]);
			ghost.m_suck = false;
			ghost.m_stun = false;
			//body.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		}
	}

}
