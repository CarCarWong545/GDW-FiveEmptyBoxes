#ifndef __MAINENTITIES_H__
#define __MAINENTITIES_H__

class MainEntities
{
public:
	//Getters
	static int MainCamera();
	static int MainPlayer();
	static int Health();
	static int Flashlight();
	static int* Enemies();
	static int Captured();
	

	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);
	static void Health(int n);
	static void Flashlight(int main);
	static void Enemies(int a[10]);
	static void Capture(int n);

	

private:
	static int m_mainCamera;
	static int m_mainPlayer;
	static int health;
	static int m_flashlight;
	static int enemies[10];
	static int captured;
	
};

#endif // !__MAINENTITIES_H__
