#include "MarioTrigger.h"
#include "ECS.h"

void MarioTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void MarioTrigger::OnEnter(int entity)
{
	Trigger::OnEnter(entity);
	MainEntities::Pickup(MainEntities::Pickups() + 1); //pickup mario item
	PhysicsBody::m_bodiesToDelete.push_back(m_triggerEntity); //deletes itself
	ECS::GetComponent<CanSwitch>(MainEntities::MainPlayer()).c_switch = true; //allow the player to switch scenes
}

void MarioTrigger::OnExit()
{
	Trigger::OnExit();
}
