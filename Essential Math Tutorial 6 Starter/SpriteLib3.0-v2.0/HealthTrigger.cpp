#include "HealthTrigger.h"
#include "ECS.h"

void HealthTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void HealthTrigger::OnEnter()
{
	Trigger::OnEnter();
	MainEntities::Health(MainEntities::Health() + 25); //recover health
	PhysicsBody::m_bodiesToDelete.push_back(m_triggerEntity); //deletes itself
}

void HealthTrigger::OnExit()
{
	Trigger::OnExit();
}
