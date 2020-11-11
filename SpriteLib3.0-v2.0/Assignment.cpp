#include "Assignment.h"
#include "Utilities.h";


double speedAmount;
double MS = 20;//maximum speed
double addSpeed;
double MAS = 5;//maximum added speed
bool isGoing = false;
bool firstSpeed = true;
bool outputPos = false;
bool outputSpeed = false;
double secSinceStart = 0;
b2Vec2 velo = b2Vec2(0.f, 0.f);
int winCounter = 0;
const int LEVEL_END = 450;
double addToJump;

Assignment::Assignment(std::string name)
	: Scene(name)

{
	//side view, so add gravity

	m_gravity = b2Vec2(0.f, -1000.f);
	m_physicsWorld->SetGravity(m_gravity);


}

void Assignment::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;
	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;




	//add in animated player
	std::string file = "spritesheets/Link.png";
	std::string anim = "A.json";
	Scene::addAnimatedMainPlayer(file, anim, 20, 30, 1.f, -450.f, 40.f, 2.f, 0, 0, m_physicsWorld);



	//ceiling and floor
	Scene::addStaticEntity("floor.png", 1000, 20, 1.f, vec3(0.f, 0.f, 2.f), 0.f, 0.f, m_physicsWorld);
	Scene::addStaticEntity("floor.png", 1000, 20, 1.f, vec3(0.f, 150.f, 2.f), 0.f, 0.f, m_physicsWorld);
	//walls
	Scene::addStaticEntity("immovable.png", 20, 150, 1.f, vec3(-500.f, 75.f, 2.f), 0.f, 0.f, m_physicsWorld);
	Scene::addStaticEntity("immovable.png", 20, 150, 1.f, vec3(500.f, 75.f, 2.f), 0.f, 0.f, m_physicsWorld);
	//level parts
	Scene::addEntity("controls.png", 100, 100, 1.f, vec3(-400.f, 40.f, 2.f));
	Scene::addEntity("finishflag.png", 30, 30, 1.f, vec3(450.f, 30.f, 2.f));
	Scene::addEntity("background.png", 1000, 150, 1.f, vec3(0.f, 70.f, 1.f));


	//level design
	//crate wall of moveables
	for (int i = 0; i < 5; i++) {
		Scene::addDynamicEntity("CursedSmile.png", 20, 20, 1.f, vec3(150.f, 20 + (i * 20), 2.f), 0.f, 0.f, m_physicsWorld, 2);
	}
	//random immovables
	Scene::addStaticEntity("immovable.png", 20, 20, 1.f, vec3(50.f, 20.f, 2.f), 0.f, 0.f, m_physicsWorld);
	Scene::addStaticEntity("immovable.png", 20, 20, 1.f, vec3(250.f, 60.f, 2.f), 0.f, 0.f, m_physicsWorld);
	Scene::addStaticEntity("immovable.png", 20, 20, 1.f, vec3(270.f, 60.f, 2.f), 0.f, 0.f, m_physicsWorld);
	Scene::addStaticEntity("immovable.png", 20, 20, 1.f, vec3(270.f, 80.f, 2.f), 0.f, 0.f, m_physicsWorld);





	//add in camera
	vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
	Scene::addMainCamera(temp, windowWidth, windowHeight);


	//set camera focus
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));



}

void Assignment::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	Scene::AdjustScrollOffset();
	player.Update();
}


void Assignment::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	isGoing = false;

	if (firstSpeed) {//speed graph A
		MS = 7;//set maximum speed (incase of toggle from speed B to A)
		//get keys
		if (Input::GetKeyDown(Key::Space)) {

			if (addToJump == 0) {//if player is not jumping (any non 0 value means theyre jumping)
				addToJump = 20;//add 20 jumping frames
			}
		}
		if (Input::GetKey(Key::A)) {
			velo += b2Vec2(-1.f, 0.f);
			isGoing = true;
		}
		if (Input::GetKey(Key::D)) {
			velo += b2Vec2(1.f, 0.f);
			isGoing = true;
		}
		if (isGoing) {//only if we are moving, add speed
			if (addSpeed + 0.1 < MAS) {//if addedspeed is less than maximum added speed
				addSpeed += 0.1;
			}
			if (addSpeed < 2) {//first part of the graph, a sharp jump
				speedAmount = sqrt(addSpeed);
			}//flat from 2 - 3.5
			if (addSpeed > 3.5) {//sharp increase from 3.5-5
				speedAmount = pow(speedAmount, 1.05);
			}
			if (speedAmount > MS) {//cant go over max speed
				speedAmount = MS;
			}
		}
		else//remove speed until 0
		{
			if (speedAmount - 1.3 > 0) {//decrease speedamount
				speedAmount -= 1.3;
			}
			else
			{
				velo = b2Vec2(0.f, 0.f);//set to 0 if its close to 0
				speedAmount = 0;
			}

			if (addSpeed - 0.5 > 0) {//decrease speed
				addSpeed -= 0.5;
			}
			else
			{
				addSpeed = 0;//set speed to 0 if its close
			}
		}

		if (addToJump - 1 > 0) {//if player is still in jumping phase
			velo += b2Vec2(0.f, 7.f);//add vector up
			if (!isGoing) {//if standing still
				speedAmount = 5;//give them temporary speed
			}

			addToJump--;//decrease counter

		}
		else
		{
			addToJump = 0;//set to 0 so that player can jump again
			velo += b2Vec2(0.f, -1.f);//add some "gravity"
		}
		player.GetBody()->SetLinearVelocity(speedAmount * velo);//move player

	}
	else//different movement type
	{
		isGoing = false;
		bool isShifting = false;//if pressing shift
		MS = 20;
		if (Input::GetKey(Key::Shift)) {
			isShifting = true;
			MS = 30;//change max speed
			isGoing = false;//if youre only pressing shift you wont start moving
		}
		//get keys
		if (Input::GetKeyDown(Key::Space)) {

			if (addToJump == 0) {//if player is not jumping (any non 0 value means theyre jumping)
				addToJump = 20;//add 20 jumping frames
			}
		}
		if (Input::GetKey(Key::A)) {
			velo += b2Vec2(-1.f, 0.f);
			isGoing = true;
		}
		if (Input::GetKey(Key::D)) {
			velo += b2Vec2(1.f, 0.f);
			isGoing = true;
		}
		if (!isGoing) {//if youre not moving, decrease speed and addedspeed
			if (speedAmount - 1 > 0) {
				speedAmount -= 1;
			}
			else//means its close enough to 0 to just make it 0
			{
				velo = b2Vec2(0.f, 0.f);
				speedAmount = 0;
			}
		}
		else//ismoving
		{
			if (isShifting) {//add extra speed for pressing shift
				speedAmount += 0.2;
			}
			speedAmount += 0.5;
			if (speedAmount > MS) {//cap the speed to maximum speed
				speedAmount = MS;
			}
		}

		if (addToJump - 1 > 0) {//if player is still in jumping phase
			velo += b2Vec2(0.f, 7.f);//add vector up
			if (!isGoing) {//if standing still
				speedAmount = 5;//give them temporary speed
			}
			addToJump--;//decrease counter

		}
		else
		{
			addToJump = 0;//set to 0 so that player can jump again
			velo += b2Vec2(0.f, -1.f);//add some "gravity"
		}
		player.GetBody()->SetLinearVelocity(speedAmount * velo);//move player

	}

	if (outputSpeed) {//if user wants to see time and speed
		if (secSinceStart == 0) {
			std::cout << "Time   speed\n";//only for first line
		}
		secSinceStart += Timer::deltaTime;
		std::cout << secSinceStart << "," << speedAmount << "\n";//output speed

	}
	else
	{
		if (!outputPos) {
			secSinceStart = 0;
		}
	}

	if (outputPos) {//if user wants to see time and position
		if (secSinceStart == 0) {
			std::cout << "Time   (X,Y)\n";//only for first line
		}
		secSinceStart += Timer::deltaTime;
		std::cout << secSinceStart << "   (" << player.GetPosition().x << "," << player.GetPosition().y << ")\n";//output speed

	}
	else
	{
		if (!outputSpeed) {
			secSinceStart = 0;
		}
	}

	if (player.GetPosition().x > LEVEL_END) {//see if player reached the end of the level
		if (winCounter < 15) {//run 15 times only, also prevents it from running more than once
			winCounter++;
			if (winCounter == 1) {//announce the end of level once
				std::cout << "The level is over!\n";
				Scene::addEntity("finish.png", 60, 30, 1.f, vec3((float)LEVEL_END, 50.f, 2.f));
				ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
			}

			//add some noids for fun! (15 times over)
			Scene::addDynamicEntity("Thenoid.png", 20, 20, 1.f, vec3((float)LEVEL_END, 70.f, 2.f), 0.f, 0.f, m_physicsWorld, 1);
			ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
			ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

		}




	}


}

void Assignment::KeyboardDown()
{


}

void Assignment::KeyboardUp()
{
	if (Input::GetKeyDown(Key::B)) {//to change speeds (changing firstSpeed's value)
		firstSpeed = !firstSpeed;
		std::cout << "Speed graph changed to: ";
		if (firstSpeed) {
			std::cout << "A\n";
		}
		else
		{
			std::cout << "B\n";
		}
	}

	if (Input::GetKeyDown(Key::C)) {//set output of time position and speed (by changing outputPos)
		outputSpeed = !outputSpeed;
		if (outputSpeed) {//if its true, we dont want both keybinds to output at the same time
			outputPos = false;
			secSinceStart = 0;//so that we can see what we are outputting
		}
	}

	if (Input::GetKeyDown(Key::V)) {//set output of time position and speed (by changing outputPos)
		outputPos = !outputPos;
		if (outputPos) {//if its true, we dont want both keybinds to output at the same time
			outputSpeed = false;
			secSinceStart = 0;//so that we can see what we are outputting
		}
	}
}
