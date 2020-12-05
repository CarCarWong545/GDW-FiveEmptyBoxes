#include "PhysicsPlaygroundListener.h"

#include "ECS.h"


PhysicsPlaygroundListener::PhysicsPlaygroundListener()
	: b2ContactListener()
{

}

void PhysicsPlaygroundListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();


	int entityA = (int)fixtureA->GetBody()->GetUserData();
	int entityB = (int)fixtureB->GetBody()->GetUserData();


	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();


	//no contact events if flashlight/main player
	
	if (entityA == MainEntities::Flashlight() && filterB.categoryBits == ENEMY)
	{
		TriggerEnter(fixtureA, fixtureB);
	}
	else if (entityB == MainEntities::Flashlight() && filterA.categoryBits == ENEMY)
	{
		TriggerEnter(fixtureB, fixtureA);
	}
	if (entityA == MainEntities::Flashlight() && filterB.categoryBits == PLAYER)
	{
		return;
	}
	else if (entityB == MainEntities::Flashlight() && filterA.categoryBits == PLAYER)
	{
		return;
	}
	if (entityA == MainEntities::Flashlight() && filterB.categoryBits == OBJECTS)
	{
		return;
	}
	else if (entityB == MainEntities::Flashlight() && filterA.categoryBits == OBJECTS)
	{
		return;
	}
	

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA, fixtureB);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB, fixtureA);
		}
	}

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump = true;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump = true;
		}
	}

}

void PhysicsPlaygroundListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
		}
	}
}

void PhysicsPlaygroundListener::TriggerEnter(b2Fixture* sensor, b2Fixture* target)
{
	int entity = (int)sensor->GetBody()->GetUserData();
	if (target)
	{
		int entity2 = (int)target->GetBody()->GetUserData();
		ECS::GetComponent<Trigger*>(entity)->OnEnter(entity2);
	}
	else
	{
		ECS::GetComponent<Trigger*>(entity)->OnEnter(0);
	}

	
}

void PhysicsPlaygroundListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
