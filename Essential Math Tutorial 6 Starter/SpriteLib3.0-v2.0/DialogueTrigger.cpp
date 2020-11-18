#include "DialogueTrigger.h"
#include "ECS.h"
#include "Player.h"
#include "Game.h"
#

void DialogueTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void DialogueTrigger::OnEnter()
{
	
	Trigger::OnEnter();
	
	if (d == 0) {
		auto& object = ECS::GetComponent<Dialouge>(m_targetEntities[0]);
		object.dialouge = true;
	}

	
}

void DialogueTrigger::OnExit()
{
	Trigger::OnExit();
}

 DialogueTrigger::DialogueTrigger(int dialouge) {

	d = dialouge;
}

