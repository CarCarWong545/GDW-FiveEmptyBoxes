#include "EnemyTrigger.h"
#include "ECS.h"

void EnemyTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void EnemyTrigger::OnEnter()
{
	Trigger::OnEnter();
	//take health - 5
	MainEntities::Health(MainEntities::Health() - 5);
	//push player back
	ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetPosition() - b2Vec2(5, 0), true);
}

void EnemyTrigger::OnExit()
{
}
