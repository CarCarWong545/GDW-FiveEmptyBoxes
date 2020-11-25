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

	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);
	static void Health(int n);
	static void Flashlight(int main);
	static void Enemies(int a[10]);

private:
	static int m_mainCamera;
	static int m_mainPlayer;
	static int health;
	static int m_flashlight;
	static int enemies[10];
};

#endif // !__MAINENTITIES_H__
