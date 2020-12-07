#pragma once 

#include "Trigger.h" 


class SavingTrigger : public Trigger
{
public:
	void OnTrigger() override;
	void OnEnter(int entity) override;
	//void OnEnter() override;
	void OnExit() override;
	void SaveData(bool ignoreStats = false);//ignoreStats = true will use the settings vector to save with, instead of the real player values at save time
	void LoadData();
	int getLuigiX();
	int getLuigiY();
	int getLuigiHP();
	int getLuigiScene();
	bool isFlashlightOn();
	int isGhostDefeated(int);//checks if ghost is defeated. a -1 return means that index is out of range
	//setters 
	void setGhostDefeated(int, bool saveDataNow = false, bool ignoreStats = false);//sets ghost at index i to be defeated (ghost must be 0 indexed)
	void setLuigiX(int, bool saveDataNow = false, bool ignoreStats = false);//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	void setLuigiY(int, bool saveDataNow = false, bool ignoreStats = false);//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	void setLuigiHP(int, bool saveDataNow = false, bool ignoreStats = false);//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	void setLuigiScene(int, bool saveDataNow = false, bool ignoreStats = false);//saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	void setFlashlightOn(bool, bool saveDataNow = false, bool ignoreStats = false); //saveDataNow is to save the value right after putting it in settings. ignoreStats is for the SaveData function. ignoreStats = true will use data from settings only, as opposed to the real player values in the game
	int numberGhostsDefeated();//returns # of ghosts defeated. not 0 indexed
	void setSettings(std::vector<int>);//sets a vector of type int to be the settings
	std::vector<int> getSettings();//returns the settings int vector
	std::vector<int> settings;//the settings themselves
	int settingsBeforeGhosts = 5;//scene,x,y,hp,flashlight
	int ghostNumber = 9; //(rooms * number of ghosts per room)

	void defaultSave();//sets up for the first save (DONT RUN THIS, ITS FOR DEBUG PURPOSES ONLY) 
protected:
	bool triggered = false; 
};

