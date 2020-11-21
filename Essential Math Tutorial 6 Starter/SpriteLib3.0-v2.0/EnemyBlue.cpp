#include "EnemyBlue.h"

EnemyBlue::EnemyBlue(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void EnemyBlue::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//IDLE ANIMATIONS\\
	
	//Idle Left
	m_animController->AddAnimation(animations["IDLELEFT"].get<Animation>()); //Idle + left = 0
	m_animController->AddAnimation(animations["IDLERIGHT"].get<Animation>()); //idle + right = 1
	m_animController->AddAnimation(animations["ATKLEFT"].get<Animation>()); //idle + equip + left= 0+0+2
	m_animController->AddAnimation(animations["ATKRIGHT"].get<Animation>()); //idle + equip + right =0+2+1 = 3
	
	//Set Default Animation
	m_animController->SetActiveAnim(IDLERIGHT);


}

void EnemyBlue::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void EnemyBlue::MovementUpdate()
{
	m_moving = false;
	if (m_physBody->GetPosition().x > -5)
	{
		m_physBody->GetBody()->SetLinearVelocity(b2Vec2(-15.f, 0.f));
		m_facing = LEFT;
	}
	if (m_physBody->GetPosition().x <= -35)
	{
		m_physBody->GetBody()->SetLinearVelocity(b2Vec2(15.f, 0.f));
		m_facing = RIGHT;
	}
}

void EnemyBlue::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = IDLE;
	}
	else //attacking
	{
		activeAnimation = ATTACK;
		m_locked = true;
		//check if the attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//will auto set to idle
			m_locked = false;
			m_attacking = false;
			//resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	
	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void EnemyBlue::SetActiveAnimation(int anim)
{
	//goes through the array from "addanimations", the order added corresponds to a number

	m_animController->SetActiveAnim(anim);
}
