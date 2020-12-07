#include "EndScreen.h"
#include "Utilities.h"

EndScreen::EndScreen(std::string name)
{
}

void EndScreen::InitScene(float windowWidth, float windowHeight)
{
	int background = 0;
	m_sceneReg = new entt::registry;
	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

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

	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "FINAL.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 220, 110);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 1.f));
		background = entity;
	}



	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
}

void EndScreen::Update()
{
}

int EndScreen::ChangeScene()
{

	return -1;


}

void EndScreen::KeyboardHold()
{
}

void EndScreen::KeyboardDown()
{
	if (Input::GetKeyDown(Key::Space) || Input::GetKeyDown(Key::Enter))
	{
		exit(0);
	}
}

void EndScreen::KeyboardUp()
{
}

int EndScreen::setX(int i) {
	return 2;
}
int EndScreen::setY(int i) {
	switch (i) {
	case 1:
		return 15;
		break;
	case 2:
		return 0;
		break;
	case 3:
		return -16;
		break;
	}
}
int EndScreen::setW(int i) {
	switch (i) {
	case 1:
		return 50;
		break;
	case 2:
		return 40;
		break;
	case 3:
		return 40;
		break;
	}
}
int EndScreen::setH(int i) {
	return 15;
}

