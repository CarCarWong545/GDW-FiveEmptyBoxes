#include "Game.h"
#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "HealthBar.h"


SavingTrigger st;

#include <random>
static int healthBar = 0;
static int healthBarBack = 0;
static int ghostBar = 0;
static int ghostBarBack = 0;
static std::vector<int> ghostCount;
VignetteEffect* ve;

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	
}
int PhysicsPlayground::ChangeScene() {
	auto& scene = ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer());
	
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

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	MainEntities::Health(100);
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	EffectManager::CreateEffect(EffectType::Vignette, windowWidth, windowHeight);
	ve = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
	changeFlashlight(false);
	//EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

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
		std::string fileName = "FOYER.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 297, 108);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 45.f, 1.f));
	}

	//Setup toad
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		//Set up the components
		std::string fileName = "toad.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 23, 26);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-16.f, 3.f, 2.f));
		ECS::GetComponent<Trigger*>(entity) = new SavingTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());//can literally be anything
	
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-16.f), float32(3.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	
	//banister
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "BANISTER.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 297, 108);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 43.f, 4.f));
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
		ECS::AttachComponent<MoveUp>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<MoveDown>(entity);
		ECS::AttachComponent<SwitchScene>(entity);
		ECS::AttachComponent<Dialouge>(entity);
		ECS::AttachComponent<CanDoor>(entity);
		ECS::AttachComponent<CanSave>(entity);
		//Sets up the components
		std::string fileName = "spritesheets/luigi.png";
		std::string animations = "Luigi.json";
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 40, 30, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity),
			&ECS::GetComponent<Transform>(entity));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(33.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}
	//e.gadd
	{

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Set up the components
		std::string fileName = "E.Gadd.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 23, 26);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
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
		tempDef.position.Set(float32(-16.f), float32(65.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}


	Scene::BoxMaker(325, 2, 30.f, 100.f, 0, 0);
	Scene::BoxMaker(325, 2, 30.f, 52.f, 0, 0);
	////Setup static Top Platform
	Scene::BoxMaker(325, 10, 30.f, -15.f, 0, 0);
	////Setup static Wall
	Scene::BoxMaker(100, 15, 185.f, 23.f, 90, 0);
	////Setup static Wall
	Scene::BoxMaker(100, 15, -125.f, 23.f, 90, 0);
	//Setup trigger
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
		ECS::GetComponent<Trigger*>(entity) = new MoveUpTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-67.f), float32(-10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	
	//Setup Move Down trigger
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
		ECS::GetComponent<Trigger*>(entity) = new MoveDownTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-67.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	

	//Setup trigger
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
		ECS::GetComponent<Trigger*>(entity) = new MoveUpTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(127.f), float32(20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
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
		tempDef.position.Set(float32(-100.f), float32(70.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}


	//Setup trigger
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
		ECS::GetComponent<Trigger*>(entity) = new MoveDownTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(127.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	{//Health bar (green)
		
		healthBar = Scene::createHealthBar();
	}

	{//health bar back (brown)
		
		healthBarBack = Scene::createHealthBarBack();
	}

	{
		ghostCount = Scene::createGhosts(10);
		ghostBar = Scene::createGhostFill();
		ghostBarBack = Scene::createGhostBack();
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	
}

void PhysicsPlayground::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto& equip = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	player.Update();

	if (!firstdialogue && deletefirstd) {
		firstdstop = (clock() - firstdstart) / CLOCKS_PER_SEC;
		if (firstdstop >= 6) {
			PhysicsBody::m_bodiesToDelete.push_back(fd);
			deletefirstd = false;
		}

	}

	if (secondd && !deletefirstd) {
		seconddstart = clock();
		sd = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Egadd Dialouge 2.png");
		secondd = false;
	}

	if (!secondd && deletesecondd) {
		secondstop = (clock() - 4.5) / CLOCKS_PER_SEC;
		if (secondstop >= secondspassforfirst) {
			PhysicsBody::m_bodiesToDelete.push_back(sd);
			deletesecondd = false;
			
		}
	}
	if (fourthd && !deletesecondd) {
		fourthstart = clock();
		ffd = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Egadd Dialouge 3.png");
		fourthd = false;
	}

	if (!fourthd && deletefourthd) {
		fourthstop = (clock() - fourthstart) / CLOCKS_PER_SEC;
		if (fourthstop >= 8.5) {
			PhysicsBody::m_bodiesToDelete.push_back(ffd);
			deletefourthd = false;
			
		}
	}
	if (fifthd && !deletefourthd) {
		fifthstart = clock();
		fffd = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Egadd Dialouge 4.png");
		fifthd = false;
	}

	if (!fifthd && deletefifthd) {
		fifthstop = (clock() - fifthstart) / CLOCKS_PER_SEC;
		if (fifthstop >= 4) {
			PhysicsBody::m_bodiesToDelete.push_back(fffd);
			deletefifthd = false;
			DialoguedoneEGadd = true;
		}
	}
	if (!thirdd) {
		thirdstop = (clock() - thirddstart) / CLOCKS_PER_SEC;
		if (thirdstop >= secondspassforfirst) {
			PhysicsBody::m_bodiesToDelete.push_back(td);
			thirdd = true;
			canmove = true;
			ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
			ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		}
	}

	if (DialoguedoneEGadd) {
		equip.m_equip = true;
		canmove = true;

		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		
		DialoguedoneEGadd = false;
		
	}

	HealthBar hb;
	hb.UpdateHealthBar(healthBar, healthBarBack);
	hb.UpdateGhostCounter(ghostCount,ghostBar,ghostBarBack);

	
	
	


}


void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	if (canmove) {
		float speed = 1.f;
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
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& canMove = ECS::GetComponent<MoveUp>(MainEntities::MainPlayer());
	auto& canMoveD = ECS::GetComponent<MoveDown>(MainEntities::MainPlayer());
	auto& isdialogue = ECS::GetComponent<Dialouge>(MainEntities::MainPlayer());
	auto& equip = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto& saveable = ECS::GetComponent<CanSave>(MainEntities::MainPlayer());
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
	if (Input::GetKeyDown(Key::G))
	{
		if (saveable.m_save) {
			
			st.defaultSave();
			st.LoadData();
		}
	}
	if (Input::GetKeyDown(Key::G))
	{
		MainEntities().Health(MainEntities().Health() - 1);
		
	}
	if (Input::GetKeyDown(Key::H))
	{
		MainEntities().Capture(MainEntities().Captured() + 1);
	}
	if (Input::GetKeyDown(Key::F))
	{
		if (isdialogue.dialouge) {
			if (firstdialogue) {
				firstdstart = clock();
				fd = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Egadd Dialouge 1.png");
				ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(fd));
				ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(fd));
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				//player.SetType(b2_staticBody);
				canmove = false;
				firstdialogue = false;
			}

		
		}

		if (saveable.m_save && thirdd) {
			
			thirddstart = clock();
			td = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Dialogue.png");
			ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(td));
			ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(td));
			st.SaveData();
			st.LoadData();
			std::cout << st.numberGhostsDefeated() << std::endl;
			thirdd = false;
			canmove = false;
		}

	}
}

void PhysicsPlayground::KeyboardUp()
{
	

}
void PhysicsPlayground::GamepadStroke(XInputController* con)
{

	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	if (canmove) 
	{
		float speed = 1.f;
		b2Vec2 vel = b2Vec2(0.f, 0.f);

		if (con->IsButtonStroked(XINPUT_GAMEPAD_DPAD_LEFT)) //move left
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
		}
		if (con->IsButtonStroked(XINPUT_GAMEPAD_DPAD_RIGHT)) //move right
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
		}
		if (con->IsButtonStroked(XINPUT_GAMEPAD_Y)) //sprint
		{
			speed *= 5.f;
		}
	}

}
void PhysicsPlayground::GamepadUp(XInputController* con)
{

}
void PhysicsPlayground::GamepadDown(XInputController* con)
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& canMove = ECS::GetComponent<MoveUp>(MainEntities::MainPlayer());
	auto& canMoveD = ECS::GetComponent<MoveDown>(MainEntities::MainPlayer());
	auto& isdialogue = ECS::GetComponent<Dialouge>(MainEntities::MainPlayer());
	auto& equip = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto& saveable = ECS::GetComponent<CanSave>(MainEntities::MainPlayer());
	auto& canDoor = ECS::GetComponent<CanDoor>(MainEntities::MainPlayer());
	auto& player2 = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	if (canJump.m_canJump)
	{
		if (con->IsButtonPressed(XINPUT_GAMEPAD_A))//presses a button - jump?
		{
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 1600000.f), true);
			canJump.m_canJump = false;
		}
	}

	if (canMove.moveUp)
	{
		if (con->IsButtonPressed(XINPUT_GAMEPAD_DPAD_UP)) //stair up
		{
			player.GetBody()->SetTransform(b2Vec2(player.GetPosition().x, player.GetPosition().y + 60), 0);
			canMove.moveUp = false;
		}
	}
	if (canMoveD.moveDown)
	{
		if (con->IsButtonPressed(XINPUT_GAMEPAD_DPAD_DOWN)) //stair down
		{
			player.GetBody()->SetTransform(b2Vec2(player.GetPosition().x, player.GetPosition().y - 70), 0);
			canMoveD.moveDown = false;
		}
	}
	if (canDoor.m_door)
	{
		auto& scene = ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer());

		if (con->IsButtonPressed(XINPUT_GAMEPAD_B)) //doors
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
	if (con->IsButtonPressed(XINPUT_GAMEPAD_X)) //talk
	{

		if (isdialogue.dialouge) {
			if (firstdialogue) {
				firstdstart = clock();
				fd = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Egadd Dialouge 1.png");
				ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(fd));
				ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(fd));
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				//player.SetType(b2_staticBody);
				canmove = false;
				firstdialogue = false;
			}


		}

		if (saveable.m_save && thirdd) {

			thirddstart = clock();
			td = Scene::DialogueMaker(200, 40, 30, 60, 5, 0, 1, "Dialogue.png");
			ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(td));
			ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(td));
			st.SaveData();
			st.LoadData();
			std::cout << st.numberGhostsDefeated() << std::endl;
			thirdd = false;
			canmove = false;
		}

	}
	if (con->IsButtonStroked(XINPUT_GAMEPAD_LEFT_SHOULDER)) //flashlight
	{
		if (player2.m_equip) {
			player2.m_flashlight = !player2.m_flashlight;

			//PhysicsPlayground::changeFlashlight(player2.m_flashlight);

		}
	}
	if (con->IsButtonStroked(XINPUT_GAMEPAD_RIGHT_SHOULDER)) //vacuum
	{
		player2.m_suck = true;
		player2.m_flashlight = false;
	}

}
void PhysicsPlayground::GamepadStick(XInputController* con)
{

}
void PhysicsPlayground::GamepadTrigger(XInputController* con)
{

}

void PhysicsPlayground::changeFlashlight(bool on) {
	if (on) {
		ve->SetInnerRadius(0.4f);
		ve->SetOuterRadius(0.5f);
		ve->SetOpacity(0.5f);
	}
	else
	{
		ve->SetInnerRadius(0.2f);
		ve->SetOuterRadius(0.3f);
		ve->SetOpacity(0.6f);

	}
	
}
