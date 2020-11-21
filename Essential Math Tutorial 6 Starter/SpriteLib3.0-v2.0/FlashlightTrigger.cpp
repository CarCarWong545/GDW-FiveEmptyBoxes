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
	if (ECS::GetComponent<Player>(m_targetEntities[1]).m_flashlight)
	{
		auto& body = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]);
		//"stun" ghost
		body.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		
		auto& ghost = ECS::GetComponent<CanDamage>(m_targetEntities[0]);
		ghost.m_candamage = false;
		ghost.m_stun = true;
	}

	
}

void FlashlightTrigger::OnExit()
{
	Trigger::OnExit();
}
