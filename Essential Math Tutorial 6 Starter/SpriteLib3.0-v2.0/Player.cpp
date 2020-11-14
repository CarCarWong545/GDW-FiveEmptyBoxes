#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
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
	m_animController->AddAnimation(animations["LuigiNoEquipmentLeft"].get<Animation>()); //value 0
	//Idle Right
	m_animController->AddAnimation(animations["LuigiNoEquipmentRight"].get<Animation>()); //value 1

	//Walk Animations\\

	//WalkLeft
	m_animController->AddAnimation(animations["LuigiWalkingLeft"].get<Animation>()); // 2+0
	//WalkRight
	m_animController->AddAnimation(animations["LuigiWalkingRight"].get<Animation>()); // 2+1

	//turn off flashlight
	m_animController->AddAnimation(animations["LuigiFlashlightOffWalkingLeft"].get<Animation>()); //4 +0
	m_animController->AddAnimation(animations["LuigiFlashlightoffWalkingRight"].get<Animation>());//4 +1

	//Set Default Animation
	m_animController->SetActiveAnim(IDLELEFT);


}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void Player::MovementUpdate()
{
	m_moving = false;

	
	if (Input::GetKey(Key::A))
	{
			//m_transform->SetPositionX(m_transform->GetPositionX() - (speed * Timer::deltaTime));
		m_facing = LEFT;
		m_moving = true;
	}
	if (Input::GetKey(Key::D))
	{
			//m_transform->SetPositionX(m_transform->GetPositionX() + (speed * Timer::deltaTime));
		m_facing = RIGHT;
		m_moving = true;
	}

	if (Input::GetKeyDown(Key::Space))
	{
		
	}
	if (Input::GetKeyDown(Key::Z))
	{
		m_flashlight = !m_flashlight;
	}
	if (Input::GetKeyDown(Key::X))
	{
		m_equip = !m_equip;
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	//if (m_moving)
	{
		//Puts it into the WALK category
		//activeAnimation = WALK;
	}
	if (!m_flashlight && m_equip)
	{
		activeAnimation = LIGHT;
	}
	else if (m_equip)
	{
		activeAnimation = EQUIP;
	}
	else
	{
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
