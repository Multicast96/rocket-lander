#include"GameMaster.h"
#include<Windows.h>
#include<string>

using namespace sf;

int main()
{
	try {
		GameMaster master(Vector2i(1280,720));
		Platform p1(Vector2f(GameMaster::getSize().x/2.0, GameMaster::getSize().y));

		Scene s1(Scene::MENU);
		s1.Add(&p1);
		s1.Add(new RocketPlayer(Vector2f(GameMaster::getSize().x/2, GameMaster::getSize().y / 2)));

		master.AddScene(&s1,true);
		master.MainLoop();
	}
	catch (std::exception &e) {
		MessageBoxA(NULL, (LPCSTR)e.what(), (LPCSTR)"Wyst¹pi³ problem :(",0);
	}
}