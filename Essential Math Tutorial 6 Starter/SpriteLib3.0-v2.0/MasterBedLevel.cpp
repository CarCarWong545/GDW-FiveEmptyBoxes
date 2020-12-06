#include "Game.h"
#include "MasterBedLevel.h"
#include "Utilities.h"


#include <random>
#include "HealthBar.h"
static int healthBarUI = 0;
static int healthBarBackUI = 0;
static int ghostBackUI = 0;
static int ghostFillUI = 0;
static std::vector<int> ghostsUI;
static HealthBar hb;



MasterBedLevel::MasterBedLevel(std::string name)
	:Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);

}


int MasterBedLevel::ChangeScene() {
	auto& scene = ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer());
	//return to beginning
	if (MainEntities::Health() <= 0)
	{
		return 0;
	}

if (scene.m_switch1)
	{
		scene.m_switch1 = false;
		return 1;
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
void MasterBedLevel::InitScene(float windowWidth, float windowHeight)
{

	curt_1 = true;
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);
	int* enemies = MainEntities::Enemies();

	if (enemies[1] != 0)
	{
		ghost_1 = true;
	}
	else
	{
		ghost_1 = false;
	}

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
		std::string fileName = "MasterBedroom.png";
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
	// ghost entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		if (ghost_1) //first enemy
		{
			auto entity = ECS::CreateEntity();
			ghost2 = entity;

			//Add components  
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			ECS::AttachComponent<CanDamage>(entity);

			ECS::GetComponent<CanDamage>(entity).m_candamage = true;

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& animController = ECS::GetComponent<AnimationController>(entity);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));
			//Sets up the components  
			std::string fileName = "spritesheets/lydia.png";
			std::string animations = "Boss2.json";

			animController.InitUVs(fileName);
			nlohmann::json animations2 = File::LoadJSON(animations);
			animController.AddAnimation(animations2["Boss2IdleBrush"].get<Animation>());
			animController.AddAnimation(animations2["Boss2FixCurtains"].get<Animation>());
			animController.AddAnimation(animations2["Boss2SuckLeft"].get<Animation>());
			animController.AddAnimation(animations2["Boss2SuckRight"].get<Animation>());
			animController.SetActiveAnim(0);

			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30, true, &animController);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


			float shrinkX = 0.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_dynamicBody;
			tempDef.position.Set(float32(145.f), float32(15.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, OBJECTS | PICKUP | TRIGGER | PTRIGGER, 0.5f, 3.f);
			//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

			tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			tempPhsBody.SetGravityScale(0.f);
			tempPhsBody.SetFixedRotation(true);
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
			ECS::AttachComponent<MoveUp>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			ECS::AttachComponent<MoveDown>(entity);
			ECS::AttachComponent<SwitchScene>(entity);
			ECS::AttachComponent<Dialouge>(entity);
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
			ECS::GetComponent<Player>(entity).m_facing = RIGHT;

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 10.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_dynamicBody;
			tempDef.position.Set(float32(-65.f), float32(30.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, OBJECTS | PICKUP | TRIGGER | GROUND | ENVIRONMENT, 0.5f, 3.f);
			//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

			tempPhsBody.SetRotationAngleDeg(0.f);
			tempPhsBody.SetFixedRotation(true);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			tempPhsBody.SetGravityScale(1.2f);

		}
		//ghost trigger entity  
		{
			/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

			if (ghost_1) //first enemy
			{

				auto entity = ECS::CreateEntity();
				ghost1 = entity;

				//Add components  
				//ECS::AttachComponent<EnemyBlue>(entity);
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);
				//ECS::AttachComponent<AnimationController>(entity);
				ECS::AttachComponent<Trigger*>(entity);
				ECS::AttachComponent<CanDamage>(entity);

				ECS::GetComponent<CanDamage>(entity).m_candamage = true;


				//Sets up the components  
				std::string fileName = "neville.png";
				//std::string animations = "BLUETWRL.json";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));
				//ECS::GetComponent<EnemyBlue>(entity).InitPlayer(fileName, animations, 40, 30, &ECS::GetComponent<Sprite>(entity),
					//&ECS::GetComponent<AnimationController>(entity),
					//&ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity));
				ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
				ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
				ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());


				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_kinematicBody;
				tempDef.position.Set(float32(145.f), float32(15.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, ETRIGGER, OBJECTS | PICKUP | TRIGGER | PTRIGGER, 0.5f, 3.f);
				//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

				tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
			}

		}
		//flashlight trigger
		{
			//Creates entity
			auto entity = ECS::CreateEntity();
			flashlight = entity;
			ECS::SetIsFlashlight(entity, true);

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
			ECS::GetComponent<Trigger*>(entity) = new FlashlightTrigger();
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(ghost1);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(ghost2);


			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = -10.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetWorldCenter().x), float32(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetWorldCenter().y));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, ENEMY | ETRIGGER);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		}
		// vacuum trigger
		{

			//Creates entity
			auto entity = ECS::CreateEntity();
			vacuum = entity;
			//ECS::SetIsFlashlight(entity, true);

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
			std::vector<int>targets = {1};
			ECS::GetComponent<Trigger*>(entity) = new VTrigger(targets);
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(ghost2);


			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = -10.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetWorldCenter().x), float32(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetWorldCenter().y));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, ETRIGGER |ENEMY);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		}
		
		Scene::BoxMaker(350, 10, 0, -15, 0, 0);
		Scene::BoxMaker(200, 10, -130, -10, 90, 0);
		Scene::BoxMaker(200, 10, 190, -10, 90, 0);

		//door trigger 1
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
			ECS::GetComponent<Trigger*>(entity) = new SceneTrigger(1);
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());



			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-115.f), float32(10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		}
		//curtains 1 joint entity
		{
			/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/
			if (ghost_1)
			{
				auto entity = ECS::CreateEntity();


				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up the components
				std::string fileName = "Curtains.png";

				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 5);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));


				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(50.f), float32(85.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER, 0.5f, 3.f);
				//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

				tempPhsBody.SetRotationAngleDeg(0.f);
				tempPhsBody.SetFixedRotation(true);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
				tempPhsBody.SetGravityScale(0.f);


				//curtain 1 entity

					/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

				auto entity2 = ECS::CreateEntity();
				curt1 = entity2;

				//Add components
				ECS::AttachComponent<Sprite>(entity2);
				ECS::AttachComponent<Transform>(entity2);
				ECS::AttachComponent<PhysicsBody>(entity2);
				ECS::AttachComponent<CanDamage>(entity2);

				ECS::GetComponent<CanDamage>(entity2).m_candamage = false;
				ECS::GetComponent<CanDamage>(entity2).m_suck = false;

				//Sets up the components
				std::string fileName2 = "Curtains.png";

				ECS::GetComponent<Sprite>(entity2).LoadSprite(fileName, 60, 50);
				ECS::GetComponent<Sprite>(entity2).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity2).SetPosition(vec3(0.f, 30.f, 3.f));


				auto& tempSpr2 = ECS::GetComponent<Sprite>(entity2);
				auto& tempPhsBody2 = ECS::GetComponent<PhysicsBody>(entity2);

				b2Body* tempBody2;
				b2BodyDef tempDef2;
				tempDef2.type = b2_dynamicBody;
				tempDef2.position.Set(float32(50.f), float32(60.f));

				tempBody2 = m_physicsWorld->CreateBody(&tempDef2);

				tempPhsBody2 = PhysicsBody(entity2, tempBody2, float(tempSpr2.GetWidth() - shrinkX), float(tempSpr2.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PICKUP, PLAYER | PTRIGGER, 0.5f, 3.f);
				//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

				tempPhsBody2.SetRotationAngleDeg(0.f);
				tempPhsBody2.SetFixedRotation(true);
				tempPhsBody2.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
				tempPhsBody2.SetGravityScale(0.f);

				b2RevoluteJointDef jointDef;
				jointDef.bodyA = tempPhsBody.GetBody();
				jointDef.bodyB = tempPhsBody2.GetBody();
				jointDef.collideConnected = true;

				jointDef.localAnchorA.Set(0, -tempSpr.GetHeight() / 2.f);
				jointDef.localAnchorB.Set(0, tempSpr2.GetHeight() / 2.f);

				b2RevoluteJoint* m_joint;

				m_joint = (b2RevoluteJoint*)m_physicsWorld->CreateJoint(&jointDef);

			}

				//curtain 2 joint entity

					/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

				auto entity3 = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity3);
				ECS::AttachComponent<Transform>(entity3);
				ECS::AttachComponent<PhysicsBody>(entity3);

				//Sets up the components
				std::string fileName3 = "Curtains.png";

				ECS::GetComponent<Sprite>(entity3).LoadSprite(fileName3, 70, 5);
				ECS::GetComponent<Sprite>(entity3).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity3).SetPosition(vec3(0.f, 30.f, 3.f));


				auto& tempSpr3 = ECS::GetComponent<Sprite>(entity3);
				auto& tempPhsBody3 = ECS::GetComponent<PhysicsBody>(entity3);


				float shrinkX = 0.f;
				float shrinkY = 0.f;

				b2Body* tempBody3;
				b2BodyDef tempDef3;
				tempDef3.type = b2_staticBody;
				tempDef3.position.Set(float32(-25.f), float32(85.f));

				tempBody3 = m_physicsWorld->CreateBody(&tempDef3);

				tempPhsBody3 = PhysicsBody(entity3, tempBody3, float(tempSpr3.GetWidth() - shrinkX), float(tempSpr3.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER, 0.5f, 3.f);
				//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

				tempPhsBody3.SetRotationAngleDeg(0.f);
				tempPhsBody3.SetFixedRotation(true);
				tempPhsBody3.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
				tempPhsBody3.SetGravityScale(0.f);


				//curtain 2 entity

					/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

				auto entity4 = ECS::CreateEntity();
				curt2 = entity4;

				//Add components
				ECS::AttachComponent<Sprite>(entity4);
				ECS::AttachComponent<Transform>(entity4);
				ECS::AttachComponent<PhysicsBody>(entity4);
				ECS::AttachComponent<CanDamage>(entity4);

				ECS::GetComponent<CanDamage>(entity4).m_candamage = false;
				ECS::GetComponent<CanDamage>(entity4).m_suck = false;

				//Sets up the components
				std::string fileName4 = "Curtains.png";

				ECS::GetComponent<Sprite>(entity4).LoadSprite(fileName4, 60, 50);
				ECS::GetComponent<Sprite>(entity4).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity4).SetPosition(vec3(0.f, 30.f, 3.f));


				auto& tempSpr4 = ECS::GetComponent<Sprite>(entity4);
				auto& tempPhsBody4 = ECS::GetComponent<PhysicsBody>(entity4);


				b2Body* tempBody4;
				b2BodyDef tempDef4;
				tempDef4.type = b2_staticBody;
				tempDef4.position.Set(float32(-25.f), float32(60.f));

				tempBody4 = m_physicsWorld->CreateBody(&tempDef4);

				tempPhsBody4 = PhysicsBody(entity4, tempBody4, float(tempSpr4.GetWidth() - shrinkX), float(tempSpr4.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | PTRIGGER, 0.5f, 3.f);
				//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

				tempPhsBody4.SetRotationAngleDeg(0.f);
				tempPhsBody4.SetFixedRotation(true);
				tempPhsBody4.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
				tempPhsBody4.SetGravityScale(0.f);

				b2RevoluteJointDef jointDef2;
				jointDef2.bodyA = tempPhsBody3.GetBody();
				jointDef2.bodyB = tempPhsBody4.GetBody();
				jointDef2.collideConnected = true;

				jointDef2.localAnchorA.Set(0, -tempSpr3.GetHeight() / 2.f);
				jointDef2.localAnchorB.Set(0, tempSpr4.GetHeight() / 2.f);

				b2RevoluteJoint* m_joint2;

				//m_joint2 = (b2RevoluteJoint*)m_physicsWorld->CreateJoint(&jointDef2);
			

		}
		// vacuum trigger 2
		{
		//Creates entity
		auto entity = ECS::CreateEntity();
		vacuum2 = entity;
		//ECS::SetIsFlashlight(entity, true);

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
		ECS::GetComponent<Trigger*>(entity) = new VTrigger2();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(curt1);
		
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = -10.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetWorldCenter().x), float32(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetWorldCenter().y));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, OBJECTS|PICKUP);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		}
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	}
	healthBarUI = Scene::createHealthBar();
	healthBarBackUI = Scene::createHealthBarBack();
	ghostBackUI = Scene::createGhostBack();
	ghostFillUI = Scene::createGhostFill();
	ghostsUI = Scene::createGhosts(13);
	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void MasterBedLevel::Update()
{


	hb.UpdateHealthBar(healthBarUI, healthBarBackUI);
	hb.UpdateGhostCounter(ghostsUI, ghostFillUI, ghostBackUI);
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	player.Update();
	int* enemies = MainEntities::Enemies();

	auto& playerb = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& players = ECS::GetComponent<Sprite>(MainEntities::MainPlayer());

	auto& light = ECS::GetComponent<PhysicsBody>(flashlight);
	auto& v = ECS::GetComponent<PhysicsBody>(vacuum);
	auto& v2 = ECS::GetComponent<PhysicsBody>(vacuum2);
	
	if (ghost_1)
	{
		if (curt_1)
		{
			if (ECS::GetComponent<CanDamage>(curt1).m_suck)
			{
				PhysicsBody::m_bodiesToDelete.push_back(curt1);
				curt_1 = false;
			}
		}
		if (curt_2)
		{
			if (ECS::GetComponent<CanDamage>(curt2).m_suck)
			{
				PhysicsBody::m_bodiesToDelete.push_back(curt2);
				curt_2 = false;
			}
		}

		auto& ghost = ECS::GetComponent<PhysicsBody>(ghost2);
		auto& c_ghost = ECS::GetComponent<CanDamage>(ghost2);
		auto& c_ghost2 = ECS::GetComponent<CanDamage>(ghost1);
		auto & anims = ECS::GetComponent<AnimationController>(ghost2);

		
		auto& ghost_2 = ECS::GetComponent<PhysicsBody>(ghost1);
		ghost_2.SetPosition(b2Vec2(ghost.GetBody()->GetWorldCenter()), false);
		ghost.GetBody()->SetAwake(true);
		ghost_2.GetBody()->SetAwake(true);

		if (c_ghost.m_candamage &&c_ghost2.m_candamage)
		{
			ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			startstuntime = clock();
		}
		else if (!c_ghost.m_stun) {
			float elapsedtime;
			float stuntime = 5.0f;
			c_ghost2.m_candamage = false;
			isstunned = true;
			if (isstunned) {
				elapsedtime = (clock() - startstuntime) / CLOCKS_PER_SEC;

				if (elapsedtime >= stuntime) {
					c_ghost.m_candamage = true;
					c_ghost.m_stun = false;
					c_ghost2.m_candamage = true;
					c_ghost2.m_stun = false;
					//ghost.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
					isstunned = false;
				}
			}
		}
		

		if (!curt_1) //curtain gone
		{
			if (!c_ghost.m_stun)
			{
				anims.SetActiveAnim(1); //second
				c_ghost.m_canbestun = true;
				c_ghost2.m_candamage = true;

				if (ghost.GetPosition().x >= 50 && ghost.GetPosition().y <= 60)
				{
					b2Vec2 direction = b2Vec2(50 - ghost.GetPosition().x, 60 - ghost.GetPosition().y);
					direction.Normalize();
					float scale = 10.f;
					direction *= scale;
					ghost.GetBody()->SetLinearVelocity(direction);
				}
				else
				{
					ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				}
			}
			if (c_ghost.m_suck && player.m_suck)
			{
				if (player.m_facing == 0) //left
				{
					anims.SetActiveAnim(2);
				}
				else //right
				{
					anims.SetActiveAnim(3);
				}
				c_ghost.m_candamage = false;
				c_ghost.m_stun = true;
				c_ghost2.m_candamage = false;
				c_ghost2.m_stun = true;
				b2Vec2 direction = b2Vec2(playerb.GetPosition().x - ghost.GetPosition().x, playerb.GetPosition().y - ghost.GetPosition().y);
				direction.Normalize();
				float scale = 10.f;
				direction *= scale;
				ghost.GetBody()->SetLinearVelocity(direction);
				ghost_2.GetBody()->SetLinearVelocity(direction);
				b2Vec2 force = direction;
				force *= 300.f;
				playerb.GetBody()->ApplyLinearImpulseToCenter(force, true);
				c_ghost.hp -= 1;

				int offset = 20; //20 is good value
				//ghost comes within offet~ of contact with vacuum
				//if ((v.GetPosition().x - offset <= ghost.GetPosition().x && ghost.GetPosition().x <= v.GetPosition().x + offset) && (v.GetPosition().y - offset <= ghost.GetPosition().y && ghost.GetPosition().y <= v.GetPosition().y + offset) || (v.GetPosition().x - offset <= ghost_2.GetPosition().x && ghost_2.GetPosition().x <= v.GetPosition().x + offset) && (v.GetPosition().y - offset <= ghost_2.GetPosition().y && ghost_2.GetPosition().y <= v.GetPosition().y + offset))
				if (c_ghost.hp <= 0)
				{
					PhysicsBody::m_bodiesToDelete.push_back(ghost1);
					PhysicsBody::m_bodiesToDelete.push_back(ghost2);
					ghost_1 = false;

					enemies[1] = 0;
					MainEntities::Enemies(enemies);
					MainEntities::Capture(MainEntities::Captured() + 1);
					//spawn heart
					{
						//Creates entity
						auto entity = ECS::CreateEntity();
						//Add components
						ECS::AttachComponent<Sprite>(entity);
						ECS::AttachComponent<Transform>(entity);
						ECS::AttachComponent<PhysicsBody>(entity);
						ECS::AttachComponent<Trigger*>(entity);

						//Sets up components
						std::string fileName = "Ghost_Heart.png";
						ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
						ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
						ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
						ECS::GetComponent<Trigger*>(entity) = new HealthTrigger();
						ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);


						auto& tempSpr = ECS::GetComponent<Sprite>(entity);
						auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

						float shrinkX = 0.f;
						float shrinkY = 0.f;
						b2Body* tempBody;
						b2BodyDef tempDef;
						tempDef.type = b2_staticBody;
						tempDef.position.Set(float32(0), float32(0));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
						tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
					}
				}

			}
			else if (c_ghost.m_suck)
			{
				c_ghost.m_stun = false;
				c_ghost2.m_stun = false;
				ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			}
			else if (!c_ghost.m_candamage && !c_ghost.m_suck)
			{
				//ghost.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
				ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			}
			
		}
		else
		{
			c_ghost.m_canbestun = false;
			c_ghost2.m_canbestun = false;
			c_ghost.m_candamage = true;
			c_ghost2.m_candamage = true;
		}
	}



	if (player.m_facing == 1)//right
	{
		light.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x + players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		v.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x + players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		v2.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x + players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		
	}
	else
	{
		light.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x - players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		v.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x - players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		v2.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x - players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		
	}
	//setup animation component again so the player doesnt lose their animations
	ECS::GetComponent<Player>(MainEntities::MainPlayer()).ReassignComponents(
		&ECS::GetComponent<AnimationController>(MainEntities::MainPlayer()),
		&ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()));
}



void MasterBedLevel::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	float speed = 1.5f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
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
}

void MasterBedLevel::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& canMove = ECS::GetComponent<MoveUp>(MainEntities::MainPlayer());
	auto& canMoveD = ECS::GetComponent<MoveDown>(MainEntities::MainPlayer());
	auto& isdialogue = ECS::GetComponent<Dialouge>(MainEntities::MainPlayer());
	auto& equip = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	auto& canDoor = ECS::GetComponent<CanDoor>(MainEntities::MainPlayer());
	auto& player2 = ECS::GetComponent<Player>(MainEntities::MainPlayer());

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
	if (canMove.moveUp)
	{
		if (Input::GetKeyDown(Key::UpArrow))
		{
			player.GetBody()->SetTransform(b2Vec2(player.GetPosition().x, player.GetPosition().y + 60), 0);
			canMove.moveUp = false;
		}
	}
	if (canMoveD.moveDown)
	{
		if (Input::GetKeyDown(Key::DownArrow))
		{
			player.GetBody()->SetTransform(b2Vec2(player.GetPosition().x, player.GetPosition().y - 70), 0);
			canMoveD.moveDown = false;
		}
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
	if (Input::GetKeyDown(Key::F))
	{
		if (isdialogue.dialouge) {
			Scene::EnviroMaker(20, 20, -5, 90, 90, 1, "PHDialogue");
			equip.m_equip = true;
		}
	}
}

void MasterBedLevel::KeyboardUp()
{


}


