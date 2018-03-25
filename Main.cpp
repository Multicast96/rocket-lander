#include"GameMaster.h"
#include<Windows.h>
#include<string>

using namespace sf;

int main()
{
	try {
		GameMaster master(Vector2i(1280,720));
		Platform p1(Vector2f(master.size.x/2, master.size.y));

		Scene s1(Scene::MENU);
		s1.Add(&p1);
		s1.Add(new RocketPlayer(Vector2f(master.size.x / 2, master.size.y / 2)));

		master.AddScene(&s1,true);
		master.MainLoop();
	}
	catch (std::exception &e) {
		MessageBoxW(NULL, (LPCWSTR)e.what(), (LPCWSTR)"Wyst¹pi³ problem :(",0);
	}
}