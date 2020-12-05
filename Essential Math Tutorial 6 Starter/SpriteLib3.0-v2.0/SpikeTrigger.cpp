#include "SpikeTrigger.h"
#include "ECS.h"
#include "Player.h"

void SpikeTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void SpikeTrigger::OnEnter(int entity)
{
	Trigger::OnEnter(entity);
	//push player back
	auto& position = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]);
	auto& player = ECS::GetComponent<Player>(m_targetEntities[0]);

	auto& ghost = ECS::GetComponent<CanDamage>(m_triggerEntity);
	if (entity == MainEntities::MainPlayer())
	{
		if (ghost.m_candamage && !ghost.m_stun)
		{
			//take health - 5
			MainEntities::Health(MainEntities::Health() - 5);
			auto direction = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->GetLinearVelocity();
			direction *= -0.5f;
			if (player.m_facing == 0)
			{
				if (position.GetPosition().x + 60 >= 130)
				{
					ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(150, position.GetPosition().y), true);
				}
				else
				{
					ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(position.GetPosition().x + 60, position.GetPosition().y - 5), true);
				}

				//ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			}
			else
			{
				if (position.GetPosition().x - 60 <= -120)
				{
					ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(-100, position.GetPosition().y), true);
				}
				else
				{
					ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(position.GetPosition().x - 60, position.GetPosition().y - 5), true);
				}

				//ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->SetLinearVelocity(direction);

			}
		}
	}
	else
	{
		auto& ghost2 = ECS::GetComponent<CanDamage>(m_targetEntities[1]);
		if (!ghost2.m_candamage) //stunned
		{
			ghost2.m_stun = true;
			//drain health
			ghost2.hp -= 40;
			auto& anims = ECS::GetComponent<AnimationController>(m_targetEntities[1]);
			if (player.m_facing == 0)
			{
				anims.SetActiveAnim(4);
			}
			else
			{
				anims.SetActiveAnim(5);
			}

			
		}
		//ghost2.m_stun = false;
	}
}

void SpikeTrigger::OnExit()
{
	Trigger::OnExit();
}
