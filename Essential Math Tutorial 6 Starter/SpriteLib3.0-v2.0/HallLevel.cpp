//#include "Game.h"
#include "Utilities.h"
#include "HallLevel.h"

HallLevel::HallLevel(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);

}

int HallLevel::ChangeScene()
{
	auto& scene = ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer());
	if (scene.m_switch3)
	{
		scene.m_switch3 = false;
		return 3;
	}
	else if (scene.m_switch0)
	{
		scene.m_switch0 = false;
		return 0;
	}
	else if (scene.m_switch2)
	{
		scene.m_switch2 = false;
		return 2;
	}
	else
	{
		return -1;
	}

}

void HallLevel::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;
	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-65.f, 65.f, -80, 25.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}
	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Hall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 297, 108);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 45.f, 1.f));
	}
	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Brown.jp";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 500, 500);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 45.f, 0.f));
	}

	//luigi entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		//ECS::AttachComponent<MoveUp>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		//ECS::AttachComponent<MoveDown>(entity);
		ECS::AttachComponent<SwitchScene>(entity);
		ECS::AttachComponent<CanDoor>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/luigi.png";
		std::string animations = "Luigi.json";
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 40, 30, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity),
			&ECS::GetComponent<Transform>(entity));

		ECS::GetComponent<Player>(entity).m_equip = true;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(34.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}
	// Floor
	Scene::BoxMaker(325, 2, 20.f, -10.f, 0, 0);

	//Wall left
	Scene::BoxMaker(325, 2, 175.f, 50.f, 90, 0);

	//Wall right
	Scene::BoxMaker(325, 2, -120.f, 50.f, 90, 0);

	//Scene::BoxMaker(325, 2, 30.f, 52.f, 0, 0);
	//door trigger to FOYER
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new SceneTrigger(2);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(70.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	//door trigger back to IDK
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new SceneTrigger(2);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-10.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	//door trigger to MasterBedroom
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new SceneTrigger(2);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-80.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	//door trigger to STUDY
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new SceneTrigger(3);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(140.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void HallLevel::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	player.Update();
}


void HallLevel::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canDoor = ECS::GetComponent<CanDoor>(MainEntities::MainPlayer());


	float speed = 1.5f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-300000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(300000.f * speed, 0.f), true);
	}

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}
	if (canDoor.m_door)
	{
		auto& scene = ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer());

		if (Input::GetKeyDown(Key::E))
		{
			if (scene.can_switch0)
			{
				scene.m_switch0 = true;
			}
			else if (scene.can_switch1)
			{
				scene.m_switch1 = true;
			}
			else if (scene.can_switch2)
			{
				scene.m_switch2 = true;
			}
			else if (scene.can_switch3)
			{
				scene.m_switch3 = true;
			}
		}
	}
}

void HallLevel::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	//auto& canMove = ECS::GetComponent<MoveUp>(MainEntities::MainPlayer());
	//auto& canMoveD = ECS::GetComponent<MoveDown>(MainEntities::MainPlayer());


	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 1600000.f), true);
			canJump.m_canJump = false;
		}
	}
}

void HallLevel::KeyboardUp()
{
}
