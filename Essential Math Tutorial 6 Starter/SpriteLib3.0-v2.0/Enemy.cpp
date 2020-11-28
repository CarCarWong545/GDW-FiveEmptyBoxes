#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, CanDamage* damage, bool hasPhys, PhysicsBody* body)
{
	InitEnemy(fileName, animationJSON, width, height, sprite, controller, transform,damage, hasPhys, body);
}

void Enemy::InitEnemy(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, CanDamage* damage, bool hasPhys, PhysicsBody* body)
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
	m_damage = damage;
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
	//m_animController->AddAnimation(animations["IDLELEFT"].get<Animation>()); //Idle + left = 0
	//m_animController->AddAnimation(animations["IDLERIGHT"].get<Animation>()); //Idle + right = 1

	//attack
	//m_animController->AddAnimation(animations["ATKLEFT"].get<Animation>()); //attack + left = 2
	//m_animController->AddAnimation(animations["ATKRIGHT"].get<Animation>()); //

	m_animController->AddAnimation(animations["LuigiNoEquipmentIdleLeft"].get<Animation>()); //Idle + left = 0
	m_animController->AddAnimation(animations["LuigiNoEquipmentIdleRight"].get<Animation>()); //idle + right = 1
	m_animController->AddAnimation(animations["LuigiFlashlightOffIdleLeft"].get<Animation>()); //idle + equip + left= 0+0+2
	m_animController->AddAnimation(animations["LuigiFlashlightoffIdleRight"].get<Animation>()); //idle + equip + right =0+2+1 = 3

	m_animController->SetActiveAnim(IDLERIGHT);
}

void Enemy::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void Enemy::MovementUpdate()
{
	m_moving = false;
	m_attacking = false;
	if (!m_damage->m_stun) //moving
	{
		m_moving = true;
		if (m_transform->GetPositionX() <= -35)
		{
			m_facing = RIGHT;
		}
		else
		{
			m_facing = LEFT;
		}
	}
	if (m_damage->is_damaging)
	{
		m_attacking = true;
		m_locked = true;
	}
}

void Enemy::AnimationUpdate()
{
	int activeAnimation = 0;
	activeAnimation = IDLE;
	if (m_attacking)
	{
		activeAnimation = 2; //attack
		//Check if the attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//Will auto set to idle
			m_locked = false;
			m_attacking = false;
			//Resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	SetActiveAnimation(activeAnimation + (int)m_facing);


}

void Enemy::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
