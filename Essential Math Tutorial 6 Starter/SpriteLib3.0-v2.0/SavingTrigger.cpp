#include "SavingTrigger.h" 
#include "ECS.h" 
#include "PhysicsPlayground.h" 
#include <fstream> 
#include "Player.h" 

void SavingTrigger::OnTrigger()
{
	Trigger::OnTrigger();

	if (!triggered)
	{

		SaveData();
	}
}

void SavingTrigger::OnEnter()
{
	Trigger::OnEnter();

	if (!triggered)
	{
		SaveData();
	}
}

void SavingTrigger::OnExit()
{
	Trigger::OnExit();
}

void SavingTrigger::SaveData()//saves current states, ignorant of settings
{
	//luigi scene 
	//luigi x 
	//luigi y 
	//luigi hp 
	// flashlight on/off 
	//ghost1 appear 
	//ghost2 appear 
	//ghost3 appear 
	//ghost4 appear 
	//ghost5 appear 
	//ghost6 appear 
	std::ofstream output;
	output.open("Luigi.txt");
	if (output.is_open()) {
		int scene = 0;
		if (ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer()).m_switch0 == true) {
			scene = 0;
		}
		if (ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer()).m_switch1 == true) {
			scene = 1;
		}
		if (ECS::GetComponent<SwitchScene>(MainEntities::MainPlayer()).m_switch2 == true) {
			scene = 2;
		}

		output << scene << std::endl;

		output << ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPosition().x << std::endl;
		output << ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPosition().y << std::endl;
		output << MainEntities().Health() << std::endl;//MainEntities Health 

		output << ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_flashlight << std::endl;

		output.close();
	}



}

void SavingTrigger::LoadData()//sets the indexes of settings
{
	settings = std::vector<int>();
	std::ifstream input;
	input.open("Luigi.txt");
	if (input.is_open()) {
		int index = 0;

		while (input.good()) {
			int setting;
			input >> setting;
			settings.push_back(setting);
			//std::cout << settings[index] << std::endl; // output for error handling 
			index++;
		}
	}
}

int SavingTrigger::getLuigiX()
{
	LoadData();
	return settings[1];
}

int SavingTrigger::getLuigiY()
{
	LoadData();
	return settings[2];
}

int SavingTrigger::getLuigiHP()
{
	LoadData();
	return settings[3];
}

int SavingTrigger::getLuigiScene()
{
	LoadData();
	return settings[0];
}

bool SavingTrigger::isFlashlightOn()//0 if true, 1 if false 
{
	LoadData();
	return settings[4];
}

int SavingTrigger::isGhostDefeated(int i) {
	LoadData();
	if (settings.size() <= (i + settingsBeforeGhosts)) {
		return settings[i + settingsBeforeGhosts];
	}
	else
	{
		return -1;//means that your index was out of range 
	}
}

void SavingTrigger::setGhostDefeated(int i) {//gets whether or not the ghost has been defeated. indexes start at 0. there is no hp storage for the ghost, only whether or not to spawn it in again.
	LoadData();
	settings[i + settingsBeforeGhosts] = 0;
	setData(settings);//lines like setData can be removed if you dont want to save progress everytime something changes.

}

void SavingTrigger::setLuigiX(int i)
{
	LoadData();
	settings[1] = i;
	setData(settings);
}

void SavingTrigger::setLuigiY(int i)
{
	LoadData();
	settings[2] = i;
	setData(settings);
}

void SavingTrigger::setLuigiHP(int i)
{
	LoadData();
	settings[3] = i;
	setData(settings);
}

void SavingTrigger::setLuigiScene(int i)
{
	LoadData();
	settings[0] = i;
	setData(settings);
}

void SavingTrigger::setFlashlightOn(bool b)
{
	LoadData();
	settings[4] = b;
	setData(settings);
}

void SavingTrigger::setData(std::vector<int> vec)//saves the settings to file
{
	std::ofstream output;
	output.open("Luigi.txt");
	if (output.is_open()) {
		for (int i : vec) {
			output << i << std::endl;
		}
		output.close();
	}

}

void SavingTrigger::defaultSave()//saves the default file, run once for debugging, and for creating a proper file.
{//DEBUG PURPOSES ONLY, DONT RUN 
	//sets values for the first save file.  
	//scene 0, 0x,30y,40hp,flashlight off, all ghosts to not be defeated 
	std::ofstream output;
	output.open("Luigi.txt");
	if (output.is_open()) {


		output << 0 << std::endl;

		output << 0 << std::endl;
		output << 30 << std::endl;
		output << 0 << std::endl;//MainEntities Health 

		output << 0 << std::endl;

		for (int i = 0; i < ghostNumber; i++) {
			output << 0 << std::endl;
		}

		output.close();
	}
}
