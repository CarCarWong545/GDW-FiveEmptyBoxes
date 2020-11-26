#include "EnemyTrigger.h"
#include "ECS.h"
#include "Player.h"

void EnemyTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void EnemyTrigger::OnEnter()
{
	Trigger::OnEnter();
	
	//push player back
	auto& position = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]);
	auto& player = ECS::GetComponent<Player>(m_targetEntities[0]);

	auto& ghost = ECS::GetComponent<CanDamage>(m_triggerEntity);
	if (ghost.m_candamage && !ghost.m_stun)
	{
		//take health - 5
		MainEntities::Health(MainEntities::Health() - 5);
		if (player.m_facing == 0)
		{
			ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(position.GetPosition().x + 30, position.GetPosition().y - 10), true);
		}
		else
		{
			ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(position.GetPosition().x - 30, position.GetPosition().y - 10), true);
		}
	}
}

void EnemyTrigger::OnExit()
{
	Trigger::OnExit();
}
