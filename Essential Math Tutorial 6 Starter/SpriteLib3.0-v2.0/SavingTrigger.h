#pragma once 

#include "Trigger.h" 

class SavingTrigger : public Trigger
{
public:
	void OnTrigger() override;
	void OnEnter() override;
	void OnExit() override;
	void SaveData();
	void LoadData();
	int getLuigiX();
	int getLuigiY();
	int getLuigiHP();
	int getLuigiScene();
	bool isFlashlightOn();
	int isGhostDefeated(int);
	//setters 
	void setGhostDefeated(int);
	void setLuigiX(int);
	void setLuigiY(int);
	void setLuigiHP(int);
	void setLuigiScene(int);
	void setFlashlightOn(bool);
	void setData(std::vector<int>);//sets data (no toad required!) 


	void defaultSave();//sets up for the first save (DONT RUN THIS, ITS FOR DEBUG PURPOSES ONLY) 
protected:
	bool triggered = false;
	std::vector<int> settings;
	int settingsBeforeGhosts = 5;
	int ghostNumber = 5 * 2; //(rooms * number of ghosts per room) 
};
