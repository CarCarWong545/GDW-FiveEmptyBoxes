#include "VTrigger2.h"
#include "ECS.h"
#include "Player.h"

void VTrigger2::OnTrigger()
{
}

void VTrigger2::OnEnter(int entity)
{
	Trigger::OnEnter(entity);
	if (ECS::GetComponent<Player>(m_targetEntities[0]).m_suck)
	{
		for (int i = 1; i < m_targetEntities.size(); i++)
		{
			if (!ECS::GetComponent<CanDamage>(m_targetEntities[i]).m_candamage)
			{
				ECS::GetComponent<CanDamage>(m_targetEntities[i]).m_suck = true;
			}
			
		}
	}
}

void VTrigger2::OnExit()
{
	Trigger::OnExit();
}
