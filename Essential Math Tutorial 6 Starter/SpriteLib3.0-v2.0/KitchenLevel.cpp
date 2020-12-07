#include "Game.h"
#include "KitchenLevel.h"
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

KitchenLevel::KitchenLevel(std::string name)
	:Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);

}
int KitchenLevel::ChangeScene() {
	auto& scene = ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer());
	//return to beginning
	if (MainEntities::Health() <= 0)
	{
		return 0;
	}

	if (scene.m_switch22)
	{
		scene.m_switch22 = false;
		return 22;
	}
	else if (scene.m_switch7)
	{
		scene.m_switch7 = false;
		return 7;
	}
	else
	{
		return -1;
	}
}

void KitchenLevel::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);
	int* enemies = MainEntities::Enemies();

	st.LoadData();
	if (st.isGhostDefeated(5) == 1)//enemies[0] != 0
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
		std::string fileName = "Kitchen.png";
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 500);
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
		ECS::AttachComponent<MoveUp>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<MoveDown>(entity);
		ECS::AttachComponent<SwitchScene>(entity);
		ECS::AttachComponent<Dialouge>(entity);
		ECS::AttachComponent<CanDoor>(entity);
		ECS::AttachComponent<CanSwitch>(entity);


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
		ECS::GetComponent<Player>(entity).m_facing = RIGHT;
		ECS::GetComponent<Player>(entity).m_flashlight = false;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-85.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, OBJECTS | PICKUP | TRIGGER | GROUND | ENVIRONMENT, 0.5f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.2f);
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
				ECS::GetComponent<CanDamage>(entity).hp = 150;

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));
				//Sets up the components  
				std::string fileName = "spritesheets/GRBRD.png";
				std::string animations = "GRBING.json";

				animController.InitUVs(fileName);
				nlohmann::json animations2 = File::LoadJSON(animations);
				animController.AddAnimation(animations2["IDLELEFT"].get<Animation>());
				animController.AddAnimation(animations2["IDLERIGHT"].get<Animation>());
				animController.AddAnimation(animations2["ATKLEFT"].get<Animation>());
				animController.AddAnimation(animations2["ATKRIGHT"].get<Animation>());
				animController.AddAnimation(animations2["GRBLEFT"].get<Animation>());
				animController.AddAnimation(animations2["GRBRIGHT"].get<Animation>());
				animController.SetActiveAnim(4);

				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30, true, &animController);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


				float shrinkX = 0.f;
				float shrinkY = 0.f;

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_dynamicBody;
				tempDef.position.Set(float32(75.f), float32(35.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, OBJECTS | PICKUP | TRIGGER | PTRIGGER, 0.5f, 3.f);
				//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
				tempPhsBody.SetGravityScale(0.f);
				tempPhsBody.SetFixedRotation(true);

			}

		}

	//ghost trigger entity  
	
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
			ECS::AttachComponent<Trigger*>(entity);
			ECS::AttachComponent<CanDamage>(entity);

			ECS::GetComponent<CanDamage>(entity).m_candamage = true; //doesnt seem to affect anything
			ECS::GetComponent<CanDamage>(entity).m_canbestun = false;

			//Sets up the components  
			std::string fileName = "neville.png";
			//std::string animations = "BLUETWRL.json";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
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
			tempDef.type = b2_kinematicBody;
			tempDef.position.Set(float32(75.f), float32(35.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, ETRIGGER, OBJECTS | PICKUP | TRIGGER | PTRIGGER, 0.5f, 3.f);
			//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);  

			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
			//tempSpr.SetTransparency(0);
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
		std::vector<int>targets = {6};
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
	Scene::BoxMaker(450, 5, 0, -13, 0, 0);
	Scene::BoxMaker(200, 10, -120, -10, 90, 0);
	Scene::BoxMaker(200, 10, 180, -10, 90, 0);

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
		ECS::GetComponent<Trigger*>(entity) = new SceneTrigger(22);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());



		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-85.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	if (ghost_1)
	{//spawn heart
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
			tempDef.position.Set(float32(75), float32(0));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		}
	}

	healthBarUI = Scene::createHealthBar();
	healthBarBackUI = Scene::createHealthBarBack();
	ghostBackUI = Scene::createGhostBack();
	ghostFillUI = Scene::createGhostFill();
	ghostsUI = Scene::createGhosts(13);

	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	if (ghost_1)
	{
		MainEntities::Health(MainEntities::Health() + 5);
	}
}

void KitchenLevel::Update()
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
	if (ghost_1)
	{
		auto& ghost = ECS::GetComponent<PhysicsBody>(ghost1);
		auto& c_ghost = ECS::GetComponent<CanDamage>(ghost2);
		auto& c_ghost_ = ECS::GetComponent<CanDamage>(ghost1);


		auto& ghost_2 = ECS::GetComponent<PhysicsBody>(ghost2);
		ghost_2.SetPosition(b2Vec2(ghost.GetBody()->GetWorldCenter()), false);
		ghost.GetBody()->SetAwake(true);
		ghost_2.GetBody()->SetAwake(true);
		auto& anims = ECS::GetComponent<AnimationController>(ghost2);
		b2Vec2 direction = b2Vec2(playerb.GetPosition().x - ghost.GetPosition().x, playerb.GetPosition().y - ghost.GetPosition().y);
		direction.Normalize();
		float scale = 25.f;
		direction *= scale;

		if (c_ghost.m_candamage && c_ghost_.m_candamage)
		{
			if (playerb.GetPosition().x >= ghost.GetPosition().x)//move right
			{

				//ghost.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
				anims.SetActiveAnim(3);
				if (playerb.GetPosition().x - 10 <= ghost.GetPosition().x && ghost.GetPosition().x <= playerb.GetPosition().x + 10)
				{
					anims.SetActiveAnim(5);
				}
			}
			if (playerb.GetPosition().x <= ghost.GetPosition().x) //left
			{
				//ghost.GetBody()->SetLinearVelocity(b2Vec2(-15, 0));
				anims.SetActiveAnim(2);
				if (playerb.GetPosition().x - 10 <= ghost.GetPosition().x && ghost.GetPosition().x <= playerb.GetPosition().x + 10)
				{
					anims.SetActiveAnim(4);
				}
			}

			c_ghost.m_canbestun = true;
			ghost.GetBody()->SetLinearVelocity(direction);
			startstuntime = clock();
		}
		else if (!c_ghost.m_stun) {
			float elapsedtime;
			float stuntime = 3.0f;
			c_ghost_.m_candamage = false;
			isstunned = true;
			ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			if (isstunned) {
				elapsedtime = (clock() - startstuntime) / CLOCKS_PER_SEC;

				if (elapsedtime >= stuntime) {
					c_ghost.m_candamage = true;
					c_ghost.m_stun = false;
					ghost.GetBody()->SetLinearVelocity(direction);
					isstunned = false;
					c_ghost_.m_candamage = true;
					c_ghost_.m_stun = false;
				}
			}
		}
		if (c_ghost.m_suck && player.m_suck)
		{
			if (player.m_facing == 0) //left
			{
				anims.SetActiveAnim(0);
			}
			else //right
			{
				anims.SetActiveAnim(1);
			}
			c_ghost.m_candamage = false;
			c_ghost.m_stun = true;
			c_ghost_.m_candamage = false;
			c_ghost_.m_stun = true;
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
				st.setGhostDefeated(5, true, true);
				st.SaveData();
				//std::cout << st.numberGhostsDefeated() << std::endl;
				std::vector<int> setting = st.getSettings();
				//for (int i : setting) {
				//	std::cout << i << std::endl;
				//}
				st.LoadData();
				PhysicsBody::m_bodiesToDelete.push_back(ghost1);
				PhysicsBody::m_bodiesToDelete.push_back(ghost2);
				ghost_1 = false;

				enemies[6] = 0; //7th enemy
				MainEntities::Enemies(enemies);
				MainEntities::Capture(MainEntities::Captured() + 1);
				//spawn mario item
				{
					ECS::GetComponent<CanSwitch>(MainEntities::MainPlayer()).c_switch = false; //can't switch once item is spawned
					//Creates entity
					auto entity = ECS::CreateEntity();
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);
					ECS::AttachComponent<Trigger*>(entity);

					//Sets up components
					std::string fileName = "Mario Glove.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
					ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
					ECS::GetComponent<Trigger*>(entity) = new MarioTrigger();
					ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);


					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(100), float32(0));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
					tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
				}
			}

		}
		else if (c_ghost.m_suck) //cant be sucked
		{
			c_ghost.m_stun = false;
			ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			if (player.m_facing == 0)
			{
				anims.SetActiveAnim(1);
			}
			else
			{
				anims.SetActiveAnim(0);
			}
			c_ghost_.m_stun = false;
		}
		else if (!c_ghost.m_candamage && !c_ghost.m_suck) //stunned
		{
			//ghost.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
			ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			if (player.m_facing == 0)
			{
				anims.SetActiveAnim(1);
			}
			else
			{
				anims.SetActiveAnim(0);
			}
		}
	}


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
	//setup animation component again so the player doesnt lose their animations
	ECS::GetComponent<Player>(MainEntities::MainPlayer()).ReassignComponents(
		&ECS::GetComponent<AnimationController>(MainEntities::MainPlayer()),
		&ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer())
	);
}



void KitchenLevel::KeyboardHold()
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

void KitchenLevel::KeyboardDown()
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
			if (ECS::GetComponent<CanSwitch>(MainEntities::MainPlayer()).c_switch)
			{

				if (scene.can_switch0)
				{
					scene.m_switch0 = true;
				}
				else if (scene.can_switch22)
				{
					scene.m_switch22 = true;
				}
				else if (scene.can_switch7)
				{
					scene.m_switch7 = true;
				}
				else if (scene.can_switch3)
				{
					scene.m_switch3 = true;
				}
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

void KitchenLevel::KeyboardUp()
{


}
