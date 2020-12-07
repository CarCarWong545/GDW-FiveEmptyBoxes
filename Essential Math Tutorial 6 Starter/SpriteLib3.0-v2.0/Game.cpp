#include "Game.h"

#include <random>


Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	//Initializes the backend with window width and height values
	BackEnd::InitBackEnd(719.f, 436.f);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//Creates a new scene.
	//Replace this with your own scene.
	m_scenes.push_back(new PhysicsPlayground("Foyer"));
	m_scenes.push_back(new HallLevel("Left Hallway"));//1
	m_scenes.push_back(new ReturnFoyer1("Foyer")); //2
	m_scenes.push_back(new StudyLevel("Study Room"));
	m_scenes.push_back(new MasterBedLevel("Master Bedroom"));
	m_scenes.push_back(new NurseryLevel("Nursery"));
	m_scenes.push_back(new HallLevel2("Right Hallway")); //6
	m_scenes.push_back(new ReturnFoyer2("Foyer ")); //7
	m_scenes.push_back(new FortuneLevel("Fortune Room"));
	m_scenes.push_back(new Washroom("Washroom"));
	m_scenes.push_back(new DiningLevel("Dining Room"));
	m_scenes.push_back(new KitchenLevel("Kitchen")); //11
	m_scenes.push_back(new SecretAltar("Secret Altar"));
	m_scenes.push_back(new Roof("Roof"));
	m_scenes.push_back(new TitleScreen("Start ")); // 14 start
	m_scenes.push_back(new InfoScene("Info ")); // 15 info
    m_scenes.push_back(new HallLevelOutofStudyRoom("Left Hallway")); //16 14 should be Main Menu, and 15 is Final Image
	m_scenes.push_back(new HallLevelOutofNursery("Left Hallway")); //17
	m_scenes.push_back(new HallLevelOutofMaster("Left Hallway")); //18
	m_scenes.push_back(new HallLevel2OutofFortune("Right Hallway")); //19
	m_scenes.push_back(new HallLevel2OutofWash("Right Hallway")); //20
	m_scenes.push_back(new HallLevel2OutofDining("Right Hallway")); //21
	m_scenes.push_back(new HallLevel2OutofKitchen("Right Hallway")); //22
	m_scenes.push_back(new EndScreen("final thanks for playing")); //23

	for (int i = 0; i < m_scenes.size(); i++)
	{
		activated[i] = false;
	}
	 
	//Sets active scene reference to our scene
	m_activeScene = m_scenes[13];

	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	//Sets m_register to point to the register in the active scene
	m_register = m_activeScene->GetScene();

	BackEnd::SetWindowName(m_activeScene->GetName());

	PhysicsSystem::Init();
}

bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with activescene clearColor
		m_window->Clear(m_activeScene->GetClearColor());
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();

		//Flips the windows
		m_window->Flip();

		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	//Update Physics System
	PhysicsSystem::Update(m_register, m_activeScene->GetPhysicsWorld());

	
	
	unsigned int index = m_activeScene->ChangeScene();
	if (index != -1) {
		m_activeScene->Unload();

		//MainEntities::ClearUI();
		//MainEntities::ClearObjects();
		m_activeScene = m_scenes[index];
		m_activeScene->InitScene(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
		m_register = m_activeScene->GetScene();
		m_window->SetWindowName(m_activeScene->GetName());
		PhysicsSystem::Init();
	}


	//Updates the active scene
	m_activeScene->Update();
}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
		MouseClick(BackEnd::GetClickEvent());

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());
}

void Game::AcceptInput()
{
	XInputManager::Update();

	//Just calls all the other input functions 
	GamepadInput();

	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::GamepadInput()
{
	XInputController* tempCon;
	//Gamepad button stroked (pressed)
	for (int i = 0; i < 3; i++)
	{
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.1f);

			//If the controller is connected, we run the different input types
			GamepadStroke(tempCon);
			GamepadUp(tempCon);
			GamepadDown(tempCon);
			GamepadStick(tempCon);
			GamepadTrigger(tempCon);
		}
	}
}

void Game::GamepadStroke(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadStroke(con);
}

void Game::GamepadUp(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadUp(con);
}

void Game::GamepadDown(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadDown(con);
}

void Game::GamepadStick(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadStick(con);
}

void Game::GamepadTrigger(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadTrigger(con);
}

void Game::KeyboardHold()
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->KeyboardHold();
}

void Game::KeyboardDown()
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->KeyboardDown();
}

void Game::KeyboardUp()
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		m_activeScene->KeyboardUp();

		if (Input::GetKeyUp(Key::F1))
		{
			m_guiActive = !m_guiActive;
		}
		if (Input::GetKeyUp(Key::P))
		{
			PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
		}
	}
}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseMotion(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseClick(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
		ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
		ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
		ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
	}

	//Resets the enabled flag
	m_click = false;
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseWheel(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}