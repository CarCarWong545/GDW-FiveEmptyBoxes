#include "Scene.h"
#include "Utilities.h"

Scene::Scene(std::string name)
{
	m_physicsWorld = new b2World(m_gravity);
	m_name = name;
}

void Scene::Unload()
{
	if (m_sceneReg != nullptr)
	{
		delete m_sceneReg;
		m_sceneReg = nullptr;
	}

	if (m_physicsWorld != nullptr)
	{
		delete m_physicsWorld;
		m_physicsWorld = nullptr;
	}
}

void Scene::InitScene(float windowWidth, float windowHeight)
{
	//Default scene class init just includes a camera so stuff doesn't immediately crash
	printf("You shouldn't be running this, while initializing this Scene Type.\n");

	//Dynamically allocates the register
	if (m_sceneReg == nullptr)
	{
		m_sceneReg = new entt::registry;
	}

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
	}

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		m_helloWorldSign = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}
}

void Scene::Update()
{
	auto& tempSpr = m_sceneReg->get<Sprite>(m_helloWorldSign);
	
	tempSpr.SetTransparency((0.5 * sin(Timer::time * 3.f)) + 0.5f);
}

void Scene::AdjustScrollOffset()
{
	float maxSizeX = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetOrthoSize().y;
	float maxSizeY = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetOrthoSize().w;

	float playerHalfSize = ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).GetWidth() / 2.f;

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset((maxSizeX * BackEnd::GetAspectRatio()) - playerHalfSize);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(maxSizeY - playerHalfSize);
}

void Scene::CreateCameraEntity(bool mainCamera, float windowWidth, float windowHeight, float left, float right, float bottom, float top, 
									float zNear, float zFar, float aspectRatio, bool vertScroll, bool horizScroll)
{
	//Setup main camera
	{
		//Creates Camera Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, false);

		ECS::AttachComponent<Camera>(entity);
		if (horizScroll)
		{
			ECS::AttachComponent<HorizontalScroll>(entity);
		}
		if (vertScroll)
		{
			ECS::AttachComponent<VerticalScroll>(entity);
		}

		vec4 temp = vec4(left, right, bottom, top);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, zNear, zFar);

		if (horizScroll)
		{
			ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		}
		if (vertScroll)
		{
			ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		}
	}
}

entt::registry* Scene::GetScene() const
{
	return m_sceneReg;
}

void Scene::SetScene(entt::registry& scene)
{
	m_sceneReg = &scene;
}

vec4 Scene::GetClearColor() const
{
	return m_clearColor;
}

void Scene::SetClearColor(vec4 clearColor)
{
	m_clearColor = clearColor;
}

std::string Scene::GetName() const
{
	return m_name;
}

void Scene::SetName(std::string name)
{
	m_name = name;
}

b2Vec2 Scene::GetGravity() const
{
	return m_gravity;
}

void Scene::SetGravity(b2Vec2 grav)
{
	m_gravity = grav;
}

b2World & Scene::GetPhysicsWorld()
{
	return *m_physicsWorld;
}

void Scene::SetWindowSize(float windowWidth, float windowHeight)
{
	//TODO: Find new way to get the main camera
	auto& tempCam = m_sceneReg->get<Camera>(MainEntities::MainCamera());
	
	tempCam.SetWindowSize(vec2(windowWidth, windowHeight));
	tempCam.Orthographic(float(windowWidth / windowHeight), tempCam.GetOrthoSize().x, tempCam.GetOrthoSize().y,
															tempCam.GetOrthoSize().z, tempCam.GetOrthoSize().w,
															tempCam.GetNear(), tempCam.GetFar());
}
void Scene::addAnimatedMainPlayer(std::string fileName, std::string aniJSON, int width, int height, float transparency, float x, float y, float z, float shrinkX, float shrinkY, b2World* m_physicsWorld) {
	//create new entity
	auto entity = ECS::CreateEntity();
	ECS::SetIsMainPlayer(entity, true);
	//add components
	ECS::AttachComponent<Player>(entity);
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<AnimationController>(entity);
	ECS::AttachComponent<Trigger>(entity);

	//set up component
	std::string picture = fileName;
	std::string animation = aniJSON;
	//initplayer
	ECS::GetComponent<Player>(entity).InitPlayer(fileName, aniJSON, height,
		width, &ECS::GetComponent<Sprite>(entity),
		&ECS::GetComponent<AnimationController>(entity),
		&ECS::GetComponent<Transform>(entity));
	//set up components
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	ECS::GetComponent<Sprite>(entity).LoadSprite(picture, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(transparency);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));
	ECS::GetComponent<Trigger>(entity).SetEntity(entity);
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	b2Body* tempBody;
	b2BodyDef tempDef;
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(x), float32(y));
	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody.SetBodyType(BodyType::BOX);

	tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, PLAYER | ENVIRONMENT | OBJECTS | ENEMY, 0.1f, 1.f);

	tempPhsBody.SetFixedRotation(true);

}

//this function actually never gets called, its just in case you want to add in a non-animated main player
void Scene::addMainPlayer(std::string fileName, int width, int height, float transparency, float x, float y, float z, float shrinkX, float shrinkY, b2World* m_physicsWorld) {
	//create new entity
	auto entity = ECS::CreateEntity();
	ECS::SetIsMainPlayer(entity, true);
	//add components
	ECS::AttachComponent<Player>(entity);
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	//set up physics and components
	std::string picture = fileName;
	ECS::GetComponent<Sprite>(entity).LoadSprite(picture, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(transparency);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));
	ECS::GetComponent<Trigger>(entity).SetEntity(entity);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	b2Body* tempBody;
	b2BodyDef tempDef;
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(x), float32(y));
	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, PLAYER | ENVIRONMENT | OBJECTS, 0.1f, 1.f);
	tempPhsBody.SetFixedRotation(true);
}

void Scene::addMainCamera(vec4 temp, float windowWidth, float windowHeight) {
	//make camera
	auto entity = ECS::CreateEntity();
	ECS::SetIsMainCamera(entity, true);
	//add components
	ECS::AttachComponent<Camera>(entity);
	ECS::AttachComponent<HorizontalScroll>(entity);
	ECS::AttachComponent<VerticalScroll>(entity);
	//set up components
	vec4 temp1 = temp;
	float aspectRatio = windowWidth / windowHeight;
	ECS::GetComponent<Camera>(entity).SetOrthoSize(temp1);
	ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
	ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp1.x, temp1.y, temp1.z, temp1.w, -100.f, 100.f);
	ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
}

void Scene::addStaticEntity(std::string file, int width, int height, float transparency, vec3 position, int sX, int sY, b2World* m_physicsWorld)
{
	auto entity = ECS::CreateEntity();
	ECS::SetIsMainPlayer(entity, false);
	//add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger>(entity);
	//set up components
	std::string fileName = file;
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(transparency);
	ECS::GetComponent<Transform>(entity).SetPosition(position);
	ECS::GetComponent<Trigger>(entity).SetEntity(entity);
	//make physics
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	b2Body* tempBody;
	b2BodyDef tempDef;
	float shrinkX = sX;
	float shrinkY = sY;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(position.x), float32(position.y));
	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENVIRONMENT | OBJECTS, 0.1f, 1.f);
}

void Scene::addDynamicEntity(std::string file, int width, int height, float transparency, vec3 position, int sX, int sY, b2World* m_physicsWorld, int type)
{
	auto entity = ECS::CreateEntity();
	ECS::SetIsMainPlayer(entity, false);
	//add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger>(entity);

	//set up components
	std::string fileName = file;
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(transparency);
	ECS::GetComponent<Transform>(entity).SetPosition(position);
	ECS::GetComponent<Trigger>(entity).SetEntity(entity);
	//make physics
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	b2Body* tempBody;
	b2BodyDef tempDef;
	float shrinkX = sX;
	float shrinkY = sY;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(position.x), float32(position.y));
	tempBody = m_physicsWorld->CreateBody(&tempDef);

	if (type == 1) {
		tempPhsBody.SetBodyType(BodyType::BOX);
		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENVIRONMENT | OBJECTS, 0.1f, 1.f);

	}
	else if (type == 2) {
		tempPhsBody.SetBodyType(BodyType::CIRCLE);
		tempPhsBody = PhysicsBody(tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENVIRONMENT | OBJECTS, 0.1f, 1.f);
	}
	else if (type == 3) {
		tempPhsBody.SetBodyType(BodyType::TRIANGLE);
		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENVIRONMENT | OBJECTS, 0.1f, 1.f);

	}
	else
	{
		tempPhsBody.SetBodyType(BodyType::BOX);
		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENVIRONMENT | OBJECTS, 0.1f, 1.f);

	}
	tempPhsBody.SetGravityScale(1.f);// 1x gravity
	//tempPhsBody.SetBodyType(BodyType::TRIANGLE);
}

void Scene::addEntity(std::string file, int width, int height, float transparency, vec3 position)
{
	//make entity
	auto entity = ECS::CreateEntity();
	//add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<Trigger>(entity);

	std::string fileName = file;
	//put entity in world
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(transparency);
	ECS::GetComponent<Transform>(entity).SetPosition(position);
	ECS::GetComponent<Trigger>(entity).SetEntity(entity);
}