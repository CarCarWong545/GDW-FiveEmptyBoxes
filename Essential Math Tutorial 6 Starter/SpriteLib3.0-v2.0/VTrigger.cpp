#include "VTrigger.h"
#include "ECS.h"
#include "Player.h"

void VTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}
VTrigger::VTrigger(std::vector<int>n)
{
	enemy = n;
}

void VTrigger::OnEnter(int entity)
{
	Trigger::OnEnter(entity);
	if (ECS::GetComponent<Player>(m_targetEntities[0]).m_suck)
	{
			auto& body = ECS::GetComponent<PhysicsBody>(entity);
			auto& ghost = ECS::GetComponent<CanDamage>(entity);
			if (!ghost.m_candamage)
			{
				ghost.m_stun = true;
				ghost.m_suck = true;

			}
	}
}

void VTrigger::OnExit()
{
	Trigger::OnExit();
	int* enemies = MainEntities::Enemies();

	for (int i = 0; i < enemy.size(); i++)
	{		
		if (enemies[enemy[i]] != 0)
			{
				auto& body = ECS::GetComponent<PhysicsBody>(m_targetEntities[i+1]);
				auto& ghost = ECS::GetComponent<CanDamage>(m_targetEntities[i+1]);
				ghost.m_suck = false;
				ghost.m_stun = false;
				//body.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			}
	}

}
