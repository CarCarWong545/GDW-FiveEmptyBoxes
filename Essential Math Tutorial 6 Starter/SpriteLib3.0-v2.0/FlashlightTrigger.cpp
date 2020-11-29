#include "FlashlightTrigger.h"
#include "ECS.h"
#include "Player.h"

void FlashlightTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void FlashlightTrigger::OnEnter()
{
	Trigger::OnEnter();
	if (ECS::GetComponent<Player>(m_targetEntities[0]).m_flashlight)
	{
		for (int i = 1; i < m_targetEntities.size(); i++)
		{
			if (ECS::GetComponent<CanDamage>(m_targetEntities[i]).m_canbestun)
			{
				auto& body = ECS::GetComponent<PhysicsBody>(m_targetEntities[i]);
				//"stun" ghost
				body.GetBody()->SetLinearVelocity(b2Vec2(0, 0));

				auto& ghost = ECS::GetComponent<CanDamage>(m_targetEntities[i]);
				ghost.m_candamage = false;
				//ghost.m_stun = true;
			}
		}
	}

	
}

void FlashlightTrigger::OnExit()
{
	Trigger::OnExit();
}
