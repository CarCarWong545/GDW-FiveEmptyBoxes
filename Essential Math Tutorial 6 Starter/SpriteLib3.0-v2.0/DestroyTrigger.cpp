#include "DestroyTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"

void DestroyTrigger::OnTrigger()
{
	Trigger::OnTrigger();

	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[i]);
		}

		triggered = true;
		
	}
}

void DestroyTrigger::OnEnter(int entity)
{
	Trigger::OnEnter(entity);

	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[i]);
		}

		triggered = true;
	}
}

void DestroyTrigger::OnExit()
{
	Trigger::OnExit();
}
