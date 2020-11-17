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
	m_animController->AddAnimation(animations["LuigiNoEquipmentIdleLeft"].get<Animation>()); //Idle + left = 0
	m_animController->AddAnimation(animations["LuigiNoEquipmentIdleRight"].get<Animation>()); //idle + right = 1
	m_animController->AddAnimation(animations["LuigiFlashlightOffIdleLeft"].get<Animation>()); //idle + equip + left= 0+0+2
	m_animController->AddAnimation(animations["LuigiFlashlightoffIdleRight"].get<Animation>()); //idle + equip + right =0+2+1 = 3
	m_animController->AddAnimation(animations["LuigiIdleLeft"].get<Animation>());//idle + light + left = 0+4+1 = 4;
	m_animController->AddAnimation(animations["LuigiIdleRight"].get<Animation>());//idle + light+ right = 5

	//Walk Animations\\

	m_animController->AddAnimation(animations["LuigiNoEquipmentLeft"].get<Animation>()); //walk + left = 6
	
	m_animController->AddAnimation(animations["LuigiNoEquipmentRight"].get<Animation>()); //7

	//turn off flashlight
	m_animController->AddAnimation(animations["LuigiFlashlightOffWalkingLeft"].get<Animation>());//walk + left + equip = 6+0+2 = 8
	m_animController->AddAnimation(animations["LuigiFlashlightoffWalkingRight"].get<Animation>()); //9

	//WalkLeft
	m_animController->AddAnimation(animations["LuigiWalkingLeft"].get<Animation>());//walk+left+light = 6+0+4 = 10 
	//WalkRight
	m_animController->AddAnimation(animations["LuigiWalkingRight"].get<Animation>()); //11

	//Set Default Animation
	m_animController->SetActiveAnim(IDLERIGHT);


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
	/*if (Input::GetKeyDown(Key::X))
	{
		m_equip = !m_equip;
	}*/
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = WALK;
	}
	else
	{
		activeAnimation = IDLE;
	}
	if (m_flashlight && m_equip)
	{
		activeAnimation += LIGHT;
	}
	else if (m_equip)
	{
		activeAnimation += EQUIP;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
