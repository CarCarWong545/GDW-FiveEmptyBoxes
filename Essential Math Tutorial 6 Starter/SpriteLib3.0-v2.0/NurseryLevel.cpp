#include "Game.h"
#include "NurseryLevel.h"
#include "Utilities.h"


#include <random>
#include "HealthBar.h"
static int healthBarUI = 0;
static int healthBarBackUI = 0;
static int ghostBackUI = 0;
static int ghostFillUI = 0;
static std::vector<int> ghostsUI;
static HealthBar hb;


static SavingTrigger st;
NurseryLevel::NurseryLevel(std::string name)
	:Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);

}
int NurseryLevel::ChangeScene() {
	auto& scene = ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer());
	//return to beginning
	if (MainEntities::Health() <= 0)
	{
		return 0;
	}

	if (scene.m_switch17)
	{
		scene.m_switch17 = false;
		return 17;
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

void NurseryLevel::InitScene(float windowWidth, float windowHeight)
{

	activate_ghost = false;
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);
	int* enemies = MainEntities::Enemies();

	st.LoadData();
	if (st.isGhostDefeated(2) == 1)//enemies[0] != 0
	{
		ghost_1 = true;
	}
	else
	{
		ghost_1 = false;
	}

	activate_ghost = false;
	second_phase = false;
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
		std::string fileName = "Nursery.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 396, 135);
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 500);
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

			ECS::GetComponent<CanDamage>(entity).m_candamage = false;

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& animController = ECS::GetComponent<AnimationController>(entity);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));
			//Sets up the components  
			std::string fileName = "spritesheets/Baby_Boss_Sheet.png";
			std::string animations = "BabyBoss.json";

			animController.InitUVs(fileName);
			nlohmann::json animations2 = File::LoadJSON(animations);
			animController.AddAnimation(animations2["BabyBossIdle"].get<Animation>());
			animController.AddAnimation(animations2["BabyBossShakeToy"].get<Animation>());
			animController.AddAnimation(animations2["BabyBossSuckLeft"].get<Animation>());
			animController.AddAnimation(animations2["BabyBossSuckRight"].get<Animation>());
			animController.AddAnimation(animations2["BabyBossFall"].get<Animation>());
			animController.SetActiveAnim(0);

			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30, true, &animController);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


			float shrinkX = -10.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_dynamicBody;
			tempDef.position.Set(float32(-120.f), float32(35.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, ENEMY, OBJECTS | PICKUP | TRIGGER | PTRIGGER, 0.5f, 3.f);
			//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

			tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			tempPhsBody.SetGravityScale(0.f);
			tempPhsBody.SetFixedRotation(true);

		}

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
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 45, 35, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity),
			&ECS::GetComponent<Transform>(entity));

		ECS::GetComponent<Player>(entity).m_equip = true;
		ECS::GetComponent<Player>(entity).m_flashlight = false;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(200.f), float32(10.f));

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

			ECS::GetComponent<CanDamage>(entity).m_candamage = false; //cant damage at beginning


			//Sets up the components  
			std::string fileName = "neville.png";
			//std::string animations = "BLUETWRL.json";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));
			//ECS::GetComponent<EnemyBlue>(entity).InitPlayer(fileName, animations, 40, 30, &ECS::GetComponent<Sprite>(entity),
				//&ECS::GetComponent<AnimationController>(entity),
				//&ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity));
			ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());


			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = -10.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_kinematicBody;
			tempDef.position.Set(float32(-130.f), float32(25.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight()-shrinkY)/2.f), vec2(0.f, 0.f), true, ETRIGGER, OBJECTS | PICKUP | TRIGGER | PTRIGGER, 0.5f, 3.f);
			//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		}

	}
	//ghost dialogue trigger
	{
		if (ghost_1)
		{//Creates entity
			auto entity = ECS::CreateEntity();

			ghostdialogue = entity;
			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<Trigger*>(entity);

			//Set up the components
			std::string fileName = "E.Gadd.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 23, 26);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-16.f, 65.f, 2.f));

			ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger(0);
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 10.f;
			float shrinkY = 10.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-120.f), float32(15.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
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
		std::vector<int>targets = {2}; //3rd enemy
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

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, ETRIGGER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	Scene::BoxMaker(450, 10, 0, -26, 0, 0);
	Scene::BoxMaker(200, 10, -170, -10, 90, 0);
	Scene::BoxMaker(200, 10, 230, -10, 90, 0);

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
		ECS::GetComponent<Trigger*>(entity) = new SceneTrigger(17);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());



		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(210.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	healthBarUI = Scene::createHealthBar();
	healthBarBackUI = Scene::createHealthBarBack();
	ghostBackUI = Scene::createGhostBack();
	ghostFillUI = Scene::createGhostFill();
	ghostsUI = Scene::createGhosts(13);

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}
void NurseryLevel::Update()
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

	if (!firstdialogue)
	{
		firststop = (clock() - firstdstart) / CLOCKS_PER_SEC;
		if (firststop >= 0.5) {
			PhysicsBody::m_bodiesToDelete.push_back(dialouge);
			firstddelete = false;
			canmove = true;
			firstdialogue = true;
			ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
			ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		}
	}
	if (!firstddelete) {
		activate_ghost = true;
		playerb.GetBody()->SetTransform(b2Vec2(0, 0), 0);
		ECS::GetComponent<CanDamage>(ghost2).m_candamage = true; //move to baby phase 1
		ECS::GetComponent<CanDamage>(ghost2).m_canbestun = true;
		ECS::GetComponent<CanDamage>(ghost1).m_candamage = true;
		PhysicsBody::m_bodiesToDelete.push_back(ghostdialogue);
		auto& anims = ECS::GetComponent<AnimationController>(ghost2);
		anims.SetActiveAnim(1);
		ECS::GetComponent<PhysicsBody>(ghost2).GetBody()->SetTransform(b2Vec2(-110, 25), 0);
		//ball entity
		{

			auto entity = ECS::CreateEntity();
			ball = entity;

			//Add components  
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<Trigger*>(entity);
			ECS::AttachComponent<CanDamage>(entity);

			//Sets up the components  
			std::string fileName = "Ball.png";

			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));
			ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());


			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_dynamicBody;
			tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(ghost2).GetPosition().x), float32((ECS::GetComponent<PhysicsBody>(ghost2).GetPosition().y)));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, ETRIGGER, PLAYER, 0.5f, 3.f);
			//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

			tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			tempPhsBody.SetGravityScale(0.f);
			tempPhsBody.SetFixedRotation(true);

			b2Vec2 direction = b2Vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - tempPhsBody.GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - tempPhsBody.GetPosition().y);
			direction.Normalize();
			float scale = 5.f;
			direction *= scale;

			tempPhsBody.GetBody()->SetLinearVelocity(direction);
		}
		firstddelete = true;
	}
		

	if (ghost_1)
	{
		auto& ghost = ECS::GetComponent<PhysicsBody>(ghost2);
		auto& c_ghost = ECS::GetComponent<CanDamage>(ghost2);
		auto& c_ghost2 = ECS::GetComponent<CanDamage>(ghost1);

		
		auto& ghost_2 = ECS::GetComponent<PhysicsBody>(ghost1);
		ghost_2.SetPosition(b2Vec2(ghost.GetBody()->GetWorldCenter()), false);
		ghost.GetBody()->SetAwake(true);
		ghost_2.GetBody()->SetAwake(true);

		if (activate_ghost) //first phase
		{
			auto& anims = ECS::GetComponent<AnimationController>(ghost2);
			auto& ball1 = ECS::GetComponent<PhysicsBody>(ball);

			if (c_ghost.m_candamage && c_ghost2.m_candamage)
			{
				ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				startstuntime = clock();

				ball1.GetBody()->SetLinearVelocity(b2Vec2(40, -5));
				if (ball1.GetBody()->GetPosition().x >= 0)
				{
					ball1.GetBody()->SetTransform(b2Vec2(float32(ghost.GetPosition().x), float32(ghost.GetPosition().y)), 0);
				}
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
						anims.SetActiveAnim(1);
					}
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

				int offset = 20;
				if (c_ghost.hp <= 0) //set up second phase of battle
				{
					second_phase = true;

					if (second_phase && seconddialogue && seconddelete) {
						secondstart = clock();
						sdialouge = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Baby2 Dialouge.png");
						seconddialogue = false;
						

					}
					
					activate_ghost = false;
					playerb.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
					playerb.GetBody()->SetTransform(b2Vec2(0, 0), 0);
					c_ghost.m_candamage = true;
					c_ghost.m_canbestun = true;
					c_ghost.hp = 100;
					c_ghost.m_stun = false;
					c_ghost.m_suck = false;

					ghost.ScaleBody(2.f, 0);
					ghost_2.ScaleBody(2.f, 0);
					ECS::GetComponent<Sprite>(ghost1).SetWidth(ghost_2.GetWidth());
					ECS::GetComponent<Sprite>(ghost1).SetHeight(ghost_2.GetHeight());
					ECS::GetComponent<Sprite>(ghost2).SetWidth(ghost.GetWidth());
					ECS::GetComponent<Sprite>(ghost2).SetHeight(ghost.GetHeight());
					PhysicsBody::m_bodiesToDelete.push_back(ball); //delete the ball
					loop_anim = 1;
					anims.GetAnimation(1).SetRepeating(true);
					anims.SetActiveAnim(1); //default anim
					ECS::GetComponent<PhysicsBody>(ghost2).GetBody()->SetTransform(b2Vec2(-110, 25), 0);
					//horse entity
					{

						auto entity = ECS::CreateEntity();
						horse = entity;

						//Add components  
						ECS::AttachComponent<Sprite>(entity);
						ECS::AttachComponent<Transform>(entity);
						ECS::AttachComponent<PhysicsBody>(entity);
						ECS::AttachComponent<Trigger*>(entity);
						ECS::AttachComponent<CanDamage>(entity);

						//Sets up the components  
						std::string fileName = "Horse.png";

						ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
						ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
						ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));
						ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
						ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
						ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());


						auto& tempSpr = ECS::GetComponent<Sprite>(entity);
						auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

						float shrinkX = 0.f;
						float shrinkY = 0.f;

						b2Body* tempBody;
						b2BodyDef tempDef;
						tempDef.type = b2_dynamicBody;
						tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(ghost2).GetPosition().x), float32(-10));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, ETRIGGER, PLAYER, 0.5f, 3.f);
						//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

						tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
						tempPhsBody.SetGravityScale(0.f);
						tempPhsBody.SetFixedRotation(true);

					}

				}

			}
			else if (c_ghost.m_suck) //player isnt sucking but can be sucked
			{
				c_ghost.m_stun = false;
				c_ghost2.m_stun = false;
				ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				anims.SetActiveAnim(1); //know he can be sucked
				ball1.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			}
			else if (!c_ghost.m_candamage && !c_ghost.m_suck) //stunned
			{
				//ghost.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
				ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				anims.SetActiveAnim(1);
				ball1.GetBody()->SetLinearVelocity(b2Vec2(40, -5));
				if (ball1.GetBody()->GetPosition().x >= 0)
				{
					//ball1.GetBody()->SetTransform(b2Vec2(float32(ghost.GetPosition().x), float32(ghost.GetPosition().y)), 0);
				}
			}
		}
		else if (second_phase) //second phase
		{
			auto& anims = ECS::GetComponent<AnimationController>(ghost2);
			auto& horse1 = ECS::GetComponent<PhysicsBody>(horse);
			
			//v.GetBody()->SetAwake(true);
			//light.GetBody()->SetAwake(true);
			//horse1.GetBody()->SetAwake(true);

			if (horse1.GetBody()->GetPosition().x >= 20)
			{
				horse1.GetBody()->SetLinearVelocity(b2Vec2(-40, 0));
			}
			if (horse1.GetPosition().x <= ghost.GetPosition().x)
			{
				horse1.GetBody()->SetLinearVelocity(b2Vec2(40, 0));
			}

			if (c_ghost.m_candamage && c_ghost2.m_candamage)
			{
				//ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				if (ghost.GetPosition().x <= -90)
				{
					ghost.GetBody()->SetLinearVelocity(b2Vec2(20, 0));
				}
				if (ghost.GetPosition().x >= -80)
				{
					ghost.GetBody()->SetLinearVelocity(b2Vec2(-20, 0));
				}

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
						//.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
						isstunned = false;
						anims.SetActiveAnim(1);
					}
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

				int offset = 20;
				if (c_ghost.hp <= 0)
				{
					st.setGhostDefeated(2, true, true);
					st.SaveData();
					//std::cout << st.numberGhostsDefeated() << std::endl;
					std::vector<int> setting = st.getSettings();
					//for (int i : setting) {
					//	std::cout << i << std::endl;
					//}
					st.LoadData();
					PhysicsBody::m_bodiesToDelete.push_back(ghost1);
					PhysicsBody::m_bodiesToDelete.push_back(ghost2);
					PhysicsBody::m_bodiesToDelete.push_back(horse);
					ghost_1 = false;
					second_phase = false;

					enemies[2] = 0;
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
						tempDef.position.Set(float32(0), float32(-15));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
						tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
					}
				}

			}
			else if (c_ghost.m_suck) //can be sucked
			{
				c_ghost.m_stun = false;
				ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				anims.SetActiveAnim(1);
			}
			else if (!c_ghost.m_candamage && !c_ghost.m_suck) //stunned
			{
				//ghost.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
				ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				anims.SetActiveAnim(1);
			}

		}

	}
	if (!seconddialogue)
	{
		secondstop = (clock() - secondstart) / CLOCKS_PER_SEC;
		if (secondstop >= 0.5) {
			PhysicsBody::m_bodiesToDelete.push_back(sdialouge);
			seconddelete = false;
			seconddialogue = true;
		}
	}
	//setup animation component again so the player doesnt lose their animations
	ECS::GetComponent<Player>(MainEntities::MainPlayer()).ReassignComponents(
		&ECS::GetComponent<AnimationController>(MainEntities::MainPlayer()),
		&ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer())
	);
	if (player.m_facing == 1)//right
	{
		light.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x + players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		v.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x + players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
	}
	else
	{
		light.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x - players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
		v.SetPosition(b2Vec2(playerb.GetBody()->GetWorldCenter().x - players.GetWidth() / 2.f, playerb.GetBody()->GetWorldCenter().y - players.GetHeight() / 5.f), false);
	}
}


void NurseryLevel::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	float speed = 1.5f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (canmove) {
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
}

void NurseryLevel::KeyboardDown()
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
			else if (scene.can_switch17)
			{
				scene.m_switch17 = true;
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
			if (firstdialogue) {
				firstdstart = clock();
				dialouge = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Baby Dialouge.png");
				ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(dialouge));
				ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(dialouge));
				firstdialogue = false;
				canmove = false;
			}
			
		
		}
	}
}

void NurseryLevel::KeyboardUp()
{


}


