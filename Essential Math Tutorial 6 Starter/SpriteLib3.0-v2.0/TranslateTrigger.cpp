#include "TranslateTrigger.h"
#include "ECS.h"

void TranslateTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void TranslateTrigger::OnEnter(int entity)
{
	Trigger::OnEnter(entity);
	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(movement, true);
	}
}

void TranslateTrigger::OnExit()
{
	Trigger::OnExit();
}
