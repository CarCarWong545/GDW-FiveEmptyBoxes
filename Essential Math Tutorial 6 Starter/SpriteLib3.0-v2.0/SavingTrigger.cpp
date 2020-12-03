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

		//SaveData();
	}
}

void SavingTrigger::OnEnter(int entity)
{
	auto& object = ECS::GetComponent<CanSave>(MainEntities::MainPlayer());
	object.m_save = true;
	Trigger::OnEnter(entity);

	

		//SaveData();

	
}

void SavingTrigger::OnExit()
{
	auto& object = ECS::GetComponent<CanSave>(MainEntities::MainPlayer());
	object.m_save = false;
	Trigger::OnExit();
}

void SavingTrigger::SaveData(bool ignoreStats)//ignoreStats ignores the players current helath in favour of whats in settings vector
{
	//Settings in order within the save file (Luigi.txt)
	//luigi scene 
	//luigi x 
	//luigi y 
	//luigi hp 
	// flashlight on/off 
	//ghost1 defeated 
	//ghost2 defeated 
	//...

	std::ofstream output;
	output.open("Luigi.txt");
	if (output.is_open()) {
		if (!ignoreStats) {//if ignoreStats is on, it will not record the players current stats,\\
			it will take them from settings instead
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
			output << int(ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPosition().x) << std::endl;
			output << int(ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPosition().y) << std::endl;
			output << MainEntities().Health() << std::endl;//MainEntities Health 

			output << int(ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_flashlight) << std::endl;
		}
		else
		{
			for (int i = 0; i < settingsBeforeGhosts; i++) {
				output << settings[i] << std::endl;
			}
		}
		

		
		int index = 0;

		for (int i : settings) {

			if (index >= settingsBeforeGhosts-1) {
				output << i << std::endl;
			}
			index++;
		}


		
	}
	output.close();


}

void SavingTrigger::LoadData()//sets the indexes of settings
{
	if (settings.size() > 0) {//if settings are empty, we will just load and save (so there are no errors!)
		settings.clear();
	}
	std::ifstream input;
	input.open("Luigi.txt");
	if (input.is_open()) {
		

		while (input.good()) {
			int setting;
			input >> setting;
			settings.push_back(setting);
			//std::cout << settings[index] << std::endl; // output for error handling 
			
		}

	}

	input.close();
}

int SavingTrigger::getLuigiX()
{//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	if (!settings.empty()) {//if settings are empty, we will fill them from the settins automatically
		LoadData();
	}
	
	return settings[1];
}

int SavingTrigger::getLuigiY()
{//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	if (!settings.empty()) {//if settings are empty, we will fill them from the settins automatically
		LoadData();
	}
	return settings[2];
}

int SavingTrigger::getLuigiHP()
{//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	if (!settings.empty()) {//if settings are empty, we will fill them from the settins automatically
		LoadData();
	}
	return settings[3];
}

int SavingTrigger::getLuigiScene()
{//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	if (!settings.empty()) {//if settings are empty, we will fill them from the settins automatically
		LoadData();
	}
	return settings[0];
}

bool SavingTrigger::isFlashlightOn()//0 if true, 1 if false 
{//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	if (!settings.empty()) {//if settings are empty, we will fill them from the settins automatically
		LoadData();
	}
	return settings[4];
}

int SavingTrigger::isGhostDefeated(int i) {//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	if (!settings.empty()) {//if settings are empty, we will fill them from the settins automatically
		LoadData();
	}
	if (settings.size() <= ((i-1) + settingsBeforeGhosts)) {//make sure your ghost i is 0 indexed!
		return settings[(i-1) + settingsBeforeGhosts];
	}
	else
	{
		return -1;//means that your index i was out of range 
	}
}

void SavingTrigger::setGhostDefeated(int i, bool saveDataNow, bool ignoreStats) {//gets whether or not the ghost has been defeated. indexes start at 0. there is no hp storage for the ghost, only whether or not to spawn it in again.
	if (!settings.empty()) {
		LoadData();
	}
	settings[(i-1) + settingsBeforeGhosts] = 0;//make sure i is 0 indexed!
	if (saveDataNow) {
		SaveData();
	}
	//setData(settings);//lines like setData can be removed if you dont want to save progress everytime something changes.

}

void SavingTrigger::setLuigiX(int i, bool saveDataNow, bool ignoreStats)
{
	if (!settings.empty()) {
		LoadData();
	}
	settings[1] = i;
	if (saveDataNow) {
		SaveData(ignoreStats);
	}
	//setData(settings);
}

void SavingTrigger::setLuigiY(int i, bool saveDataNow, bool ignoreStats)
{
	if (!settings.empty()) {
		LoadData();
	}
	settings[2] = i;
	if (saveDataNow) {
		SaveData(ignoreStats);
	}
	//setData(settings);
}

void SavingTrigger::setLuigiHP(int i, bool saveDataNow, bool ignoreStats)
{
	if (!settings.empty()) {
		LoadData();
	}
	settings[3] = i;
	if (saveDataNow) {
		SaveData(ignoreStats);
	}
	//setData(settings);
}

void SavingTrigger::setLuigiScene(int i, bool saveDataNow, bool ignoreStats)
{
	if (!settings.empty()) {
		LoadData();
	}
	settings[0] = i;
	if (saveDataNow) {
		SaveData(ignoreStats);
	}
	//setData(settings);
}

void SavingTrigger::setFlashlightOn(bool b, bool saveDataNow, bool ignoreStats)
{
	if (!settings.empty()) {
		LoadData();
	}
	settings[4] = b;
	if (saveDataNow) {
		SaveData(ignoreStats);
	}
	//setData(settings);
}

int SavingTrigger::numberGhostsDefeated()
{//returns # of ghosts defeated.
	int ghostsDefeated = 0;
	

	for (int i = settingsBeforeGhosts-1; i < settings.size()-1; i++) {
		if (settings[i] == 0) {
			ghostsDefeated++;
			continue;
		}
	}
	return ghostsDefeated;
}

void SavingTrigger::setSettings(std::vector<int> vec)
{
	settings = vec;
}

std::vector<int> SavingTrigger::getSettings()
{
	return settings;
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
		output << 40 << std::endl;//MainEntities Health 

		output << 1 << std::endl;

		for (int i = 0; i < ghostNumber; i++) {
			output << 1 << std::endl;
		}

		output.close();
	}
}
